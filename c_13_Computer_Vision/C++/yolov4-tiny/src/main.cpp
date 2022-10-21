#include <iostream>
#include <unistd.h>
#include <thread>   
#include <stdio.h>
#include <iomanip>
#include <fstream>
#include <set>

#include <yolov4_tiny.hpp>

#include <Hungarian.hpp>
#include <KalmanTracker.hpp>

// #include<tracking.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>




double GetIOU(Rect_<float> bb_test,Rect_<float>bb_gt)
{
    float in =(bb_test & bb_gt).area();
    float un =bb_test.area() + bb_gt.area() -in;
    if(un<DBL_EPSILON)
    {
        return 0;
    
    }
    return (double)(in/ un);
}










static void draw_objects(const cv::Mat& image, const std::vector<TrackingBox>& objects,const std::vector<TrackingBox>& obj_labels)
{
    
    // cv::Mat image=bgr.clone();

    for (size_t i = 0; i < objects.size(); i++)
    {
        const TrackingBox& obj = objects[i];
        const TrackingBox& obj_l = obj_labels[i];
        fprintf(stderr, "%s = %.4f at %f %f %lf x %lf\n", obj_l.label.c_str(), obj_l.score,
                obj.box.x, obj.box.y, obj.box.width, obj.box.height);

        cv::rectangle(image, obj.box, cv::Scalar(255, 0, 0));

        char text[256];
        sprintf(text, "%d %s %.2f%%",obj.id , obj_l.label.c_str(), obj_l.score * 100);

        int baseLine = 0;
        cv::Size label_size = cv::getTextSize(text, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);

        int x = obj.box.x;
        int y = obj.box.y - label_size.height - baseLine;
        if (y < 0)
            y = 0;
        if (x + label_size.width > image.cols)
            x = image.cols - label_size.width;

        cv::rectangle(image, cv::Rect(cv::Point(x, y), 
                    cv::Size(label_size.width, label_size.height + baseLine)),
                    cv::Scalar(255, 255, 255), -1);

        cv::putText(image, text, cv::Point(x, y + label_size.height),
                    cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));
        
    }
    // bgr=image;
}

void Yolov4Tiny()
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
    
    //check video
    string name="y2meta";
    std::string path_video="../data/video/"+name+".mp4";
    cv::VideoCapture video;
    cv::Mat Frame;

    video.open(path_video);
    // cv::namedWindow("VideoCapture",cv::WINDOW_AUTOSIZE);
    if(!video.isOpened())
    {
        std::cout<<"Can't open video"<<std::endl;
    }
    else
    {
        int frameWidth = video.get(cv::CAP_PROP_FRAME_WIDTH);
	    int frameHeigth = video.get(cv::CAP_PROP_FRAME_HEIGHT);

        cv::VideoWriter output(	"../data/output/"+name+".avi", 
				cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 
				30, 
				cv::Size(frameWidth, frameHeigth));

        ofstream resultsFile;
        string resFileName = "../data/output/test_file.txt";
	    resultsFile.open(resFileName);
        int start=0;
        double cycle_time = 0.0;
	    int64 start_time = 0;

        while(video.read(Frame))
        {
            start_time =cv::getTickCount();
            
            float threshold = 0.5f;
            std::vector<TrackingBox> objects;
            
            yolov4_tiny->runNcnnDetect(Frame, labels, objects, threshold,start+1);

            std::cout<<"Frame: "<<start+1<<"  --detect: "<< objects.size()<<std::endl;
            

            
            vector<vector<TrackingBox>> detFrameData;
	        //  std::cout<<start<<" -1-   "<<detFrameData[start].size()<<std::endl;
                
            detFrameData.push_back(objects);
            
            if(objects.size() !=0)
            {
                int frame_count = 0;
                int max_age = 1;
                int min_hits = 3;
                double iouThreshold = 0.3;
                vector<KalmanTracker> trackers;
                KalmanTracker::kf_count = 0; // tracking id relies on this, so we have to reset it in each seq.
                vector<Rect_<float>> predictedBoxes;
                vector<vector<double>> iouMatrix;
                vector<int> assignment;
                set<int> unmatchedDetections;
                set<int> unmatchedTrajectories;
                set<int> allItems;
                set<int> matchedItems;
                vector<cv::Point> matchedPairs;
                vector<TrackingBox> frameTrackingResult;
                unsigned int trkNum = 0;
                unsigned int detNum = 0;
                ofstream resultsFile;
                string resFileName = "../data/output/test_file.txt";
                resultsFile.open(resFileName);
                if (!resultsFile.is_open())
                {
                    cerr << "Error: can not create file " << resFileName << endl;
                    return;
                }
                

                
                
            
                // initialize kalman trackers using first detections.
                for (unsigned int i = 0; i < detFrameData.back().size(); i++)
                {
                    
                    KalmanTracker trk = KalmanTracker(detFrameData.back()[i].box);
                    trackers.push_back(trk);
                }
                            
                                                                    
                predictedBoxes.clear();

                for (auto it = trackers.begin(); it != trackers.end();)
                {
                    Rect_<float> pBox = (*it).predict();
                    if (pBox.x >= 0 && pBox.y >= 0)
                    {
                        predictedBoxes.push_back(pBox);
                        it++;
                    }
                    else
                    {
                        it = trackers.erase(it);
                        //cerr << "Box invalid at frame: " << frame_count << endl;
                    }
                }
                
                trkNum = predictedBoxes.size();
                
                detNum = detFrameData.back().size();
                
                iouMatrix.clear();
                iouMatrix.resize(trkNum, vector<double>(detNum, 0));
                
                for (unsigned int i = 0; i < trkNum; i++) // compute iou matrix as a distance matrix
                {
                    for (unsigned int j = 0; j < detNum; j++)
                    {
                        // use 1-iou because the hungarian algorithm computes a minimum-cost assignment.
                        iouMatrix[i][j] = 1 - GetIOU(predictedBoxes[i], detFrameData.back()[j].box);
                    }
                }
                
                HungarianAlgorithm HungAlgo;
                HungAlgo.Solve(iouMatrix, assignment);
                
            
                // find matches, unmatched_detections and unmatched_predictions
                unmatchedTrajectories.clear();
                
                unmatchedDetections.clear();
                
                allItems.clear();
                
                matchedItems.clear();

                
                if (detNum > trkNum) //	there are unmatched detections
                {
                    for (unsigned int n = 0; n < detNum; n++)
                        allItems.insert(n);

                    for (unsigned int i = 0; i < trkNum; ++i)
                        matchedItems.insert(assignment[i]);

                    set_difference(allItems.begin(), allItems.end(),
                        matchedItems.begin(), matchedItems.end(),
                        insert_iterator<set<int>>(unmatchedDetections, unmatchedDetections.begin()));
                }
                else if (detNum < trkNum) // there are unmatched trajectory/predictions
                {
                    for (unsigned int i = 0; i < trkNum; ++i)
                        if (assignment[i] == -1) // unassigned label will be set as -1 in the assignment algorithm
                            unmatchedTrajectories.insert(i);
                }
                else
                    ;

                // filter out matched with low IOU
                matchedPairs.clear();
                for (unsigned int i = 0; i < trkNum; ++i)
                {
                    if (assignment[i] == -1) // pass over invalid values
                        continue;
                    if (1 - iouMatrix[i][assignment[i]] < iouThreshold)
                    {
                        unmatchedTrajectories.insert(i);
                        unmatchedDetections.insert(assignment[i]);
                    }
                    else
                        matchedPairs.push_back(cv::Point(i, assignment[i]));
                }
                
                int detIdx, trkIdx;
                for (unsigned int i = 0; i < matchedPairs.size(); i++)
                {
                    trkIdx = matchedPairs[i].x;
                    detIdx = matchedPairs[i].y;
                    trackers[trkIdx].update(detFrameData.back()[detIdx].box);
                }

                // create and initialise new trackers for unmatched detections
                for (auto umd : unmatchedDetections)
                {
                    KalmanTracker tracker = KalmanTracker(detFrameData.back()[umd].box);
                    trackers.push_back(tracker);
                }
                
                // get trackers' output
                frameTrackingResult.clear();
                for (auto it = trackers.begin(); it != trackers.end();)
                {
                    if (((*it).m_time_since_update < 1) &&
                        ((*it).m_hit_streak >= min_hits || frame_count <= min_hits))
                    {
                        TrackingBox res;
                        res.box = (*it).get_state();
                        res.id = (*it).m_id + 1;
                        res.frame = frame_count;
                        // res.score = (*it).score;
                        // res.label= (*it).label;
                        // res.obj_id= (*it).obj_id;
                        frameTrackingResult.push_back(res);
                        it++;
                    }
                    else
                        it++;

                    // remove dead tracklet
                    if (it != trackers.end() && (*it).m_time_since_update > max_age)
                        it = trackers.erase(it);
                }
                
                cycle_time = (double)(getTickCount() - start_time);
                // total_time += cycle_time / getTickFrequency();

                
                draw_objects(Frame,frameTrackingResult,objects);

            }
            
            
            
            // std::cout << "FPS: "  << video.get(CV_CAP_PROP_FPS) << std::endl;

            ++start;
            cv::imshow("Video test",Frame);
            output.write(Frame);
            // int k=cv::waitKey(0);
            if(cv::waitKey(30) >= 0) break;

            
        }
    }



    // int n = 1;
    // auto start = std::chrono::high_resolution_clock::now();
    // // start time
    // while(n <= 100) {

    // float threshold = 0.7f;
    // std::vector<strObject> objects;
    // yolov4_tiny->runNcnnDetect(img, labels, objects, threshold);
    // std::cout << n << " -- detect " << objects.size() << std::endl;

    // n += 1;
    // }
    // auto end = std::chrono::high_resolution_clock::now();
    // // end time
    // auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    // auto av = time.count() / n;
    // std::cout << "duration = " << av << std::endl;
    // std::cout << "FPS = " << 1000.f / av << std::endl;

    // draw(img, objects);
    // cv::imwrite("/data/local/tmp/outputs/out.jpg", img);
}







int main()
{
    std::thread threadFunc(Yolov4Tiny);
    threadFunc.detach();
    while(1) {
        sleep(100);
    }

    return 0;
}