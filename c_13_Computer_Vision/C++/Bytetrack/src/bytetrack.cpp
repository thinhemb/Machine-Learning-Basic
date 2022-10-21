#include "layer.h"
#include "net.h"


#if defined(USE_NCNN_SIMPLEOCV)
#include "simpleocv.h"
#include <opencv2/opencv.hpp>
#else
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#endif
#include <float.h>
#include <stdio.h>
#include <vector>
#include <chrono>
#include <thread> 
#include <unistd.h>

#include "BYTETracker.h"
#include <yolov4_tiny.hpp>
#include <struct.hpp>

#define INPUT_W 1088  // target image size w after resize
#define INPUT_H 608   // target image size h after resize


//// function resize image
cv::Mat static_resize(cv::Mat& img) {
    float r = std::min(INPUT_W / (img.cols*1.0), INPUT_H / (img.rows*1.0));
    // r = std::min(r, 1.0f);
    int unpad_w = r * img.cols;
    int unpad_h = r * img.rows;
    cv::Mat re(unpad_h, unpad_w, CV_8UC3);
    cv::resize(img, re, re.size());
    cv::Mat out(INPUT_H, INPUT_W, CV_8UC3, cv::Scalar(114, 114, 114));
    re.copyTo(out(cv::Rect(0, 0, re.cols, re.rows)));
    return out;
}
static void draw_objects(const cv::Mat& img,const std::vector<Object>& objects,const std::vector<STrack> output_stracks,int frame,BYTETracker tracker)
{


    for (int i = 0; i < output_stracks.size(); i++)
		{
			std::vector<float> tlwh = output_stracks[i].tlwh;

            
			bool vertical = tlwh[2] / tlwh[3] > 1.6;
			if (tlwh[2] * tlwh[3] > 20 && !vertical)
			{
				cv::Scalar s = tracker.get_color(output_stracks[i].track_id);

                fprintf(stderr, "%s = %.4f at %f %f %lf x %lf\n", objects[i].label.c_str(), objects[i].prob,
                objects[i].rect.x, objects[i].rect.y, objects[i].rect.width, objects[i].rect.height);

                char text[256];
                sprintf(text, "%d %s %.2f%%",output_stracks[i].track_id , objects[i].label.c_str(), objects[i].prob * 100);

				putText(img,  text, cv::Point(tlwh[0], tlwh[1] - 5), 
                        0, 0.6, cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
                rectangle(img, cv::Rect(tlwh[0], tlwh[1], tlwh[2], tlwh[3]), s, 2);
			}
		}

}

void Yolov4_tiny_bytetrack()
{
    


    NcnnDetect *yolov4_tiny = new NcnnDetect();
    yolov4_tiny->initNcnnDetect("../data/models/yolov4-tiny-opt.bin", "../data/models/yolov4-tiny-opt.param");
    std::vector<std::string> labels={"background", "person", "bicycle",
                                        "car", "motorbike", "aeroplane", "bus", "train", "truck",
                                        "boat", "traffic light", "fire hydrant", "stop sign",
                                        "parking meter", "bench", "bird", "cat", "dog", "horse",
                                        "sheep", "cow", "elephant", "bear", "zebra", "giraffe",
                                        "backpack", "umbrella", "handbag", "tie", "suitcase",
                                        "frisbee", "skis", "snowboard", "sports ball", "kite",
                                        "baseball bat", "baseball glove", "skateboard", "surfboard",
                                        "tennis racket", "bottle", "wine glass", "cup", "fork",
                                        "knife", "spoon", "bowl", "banana", "apple", "sandwich",
                                        "orange", "broccoli", "carrot", "hot dog", "pizza", "donut",
                                        "cake", "chair", "sofa", "pottedplant", "bed", "diningtable",
                                        "toilet", "tvmonitor", "laptop", "mouse", "remote", "keyboard",
                                        "cell phone", "microwave", "oven", "toaster", "sink",
                                        "refrigerator", "book", "clock", "vase", "scissors",
                                        "teddy bear", "hair drier", "toothbrush"
                                       };
    
    

    const char* videopath = "../data/video/y2meta.mp4";
    


    cv::VideoCapture cap(videopath);
	if (!cap.isOpened())
    {
        std::cout<<"video is error /n";
        return;
    }
		

	int img_w = cap.get(cv::CAP_PROP_FRAME_WIDTH);
	int img_h = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    int fps = cap.get(cv::CAP_PROP_FPS);
    long nFrame = static_cast<long>(cap.get(cv::CAP_PROP_FRAME_COUNT));
    std::cout << "Total frames: " << nFrame << std::endl;

    cv::VideoWriter writer("../data/output/demo.mp4", 
            cv::VideoWriter::fourcc('m', 'p', '4', 'v'), 
            fps, 
            cv::Size(img_w, img_h));

    cv::Mat img;
    BYTETracker tracker(fps, 30);
    int num_frames = 0;
    int total_ms = 1;
    float threshold = 0.2f;
	for (;;)
    {
        if(!cap.read(img))
            break;
        num_frames ++;
        if (num_frames % 30 == 0)
        {
            std::cout << "Processing frame " << num_frames << " (" << num_frames * 1000000 / total_ms << " fps)" << std::endl;
        }
        if(num_frames%3==0 or num_frames%4==0)
        {
            continue;
        }
		if (img.empty())
			break;

      
        auto start = std::chrono::system_clock::now();

        std::vector<Object> objects;
        yolov4_tiny->runNcnnDetect(img,labels,objects,threshold,num_frames);

        
        std::vector<STrack> output_stracks = tracker.update(objects);
        auto end = std::chrono::system_clock::now();
        total_ms = total_ms + std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        draw_objects(img,objects,output_stracks,num_frames,tracker);
        std::cout<<"Frame: "<<num_frames<<"  --detect: "<< objects.size()<<std::endl;
        putText(img, cv::format("frame: %d fps: %d num: %ld", num_frames, num_frames * 1000000 / total_ms, output_stracks.size()), 
                cv::Point(0, 30), 0, 0.6, cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
        cv::imshow("video test",img);
        writer.write(img);
        
        char c = cv::waitKey(1);
        if (c > 0)
        {
            break;
        }
    }
    cap.release();
    std::cout << "FPS: " << num_frames * 1000000 / total_ms << std::endl;

    
}




int main()
{
    std::thread threadFunc(Yolov4_tiny_bytetrack);
    threadFunc.detach();
    while(1) {
        sleep(100);
    }

    return 0;
}
