//
// Created by ubuntu on 1/20/23.
//
#include <iostream>
#include "chrono"
#include "yolov8.hpp"
#include "opencv2/opencv.hpp"
#include "BYTETracker.h"
#include "STrack.h"

using namespace std;

const std::vector<std::string> CLASS_NAMES = {"Glass", "Paper", "Metal", "Plastic", "Battery", "Organic"};

const std::vector<std::vector<unsigned int>> COLORS = {
	{0, 114, 189}, {217, 83, 25}, {237, 177, 32}, {126, 47, 142}, {119, 172, 48}, {77, 190, 238}, {162, 20, 47}, {76, 76, 76}, {153, 153, 153}, {255, 0, 0}, {255, 128, 0}, {191, 191, 0}, {0, 255, 0}, {0, 0, 255}, {170, 0, 255}, {85, 85, 0}, {85, 170, 0}, {85, 255, 0}, {170, 85, 0}, {170, 170, 0}, {170, 255, 0}, {255, 85, 0}, {255, 170, 0}, {255, 255, 0}, {0, 85, 128}, {0, 170, 128}, {0, 255, 128}, {85, 0, 128}, {85, 85, 128}, {85, 170, 128}, {85, 255, 128}, {170, 0, 128}, {170, 85, 128}, {170, 170, 128}, {170, 255, 128}, {255, 0, 128}, {255, 85, 128}, {255, 170, 128}, {255, 255, 128}, {0, 85, 255}, {0, 170, 255}, {0, 255, 255}, {85, 0, 255}, {85, 85, 255}, {85, 170, 255}, {85, 255, 255}, {170, 0, 255}, {170, 85, 255}, {170, 170, 255}, {170, 255, 255}, {255, 0, 255}, {255, 85, 255}, {255, 170, 255}, {85, 0, 0}, {128, 0, 0}, {170, 0, 0}, {212, 0, 0}, {255, 0, 0}, {0, 43, 0}, {0, 85, 0}, {0, 128, 0}, {0, 170, 0}, {0, 212, 0}, {0, 255, 0}, {0, 0, 43}, {0, 0, 85}, {0, 0, 128}, {0, 0, 170}, {0, 0, 212}, {0, 0, 255}, {0, 0, 0}, {36, 36, 36}, {73, 73, 73}, {109, 109, 109}, {146, 146, 146}, {182, 182, 182}, {219, 219, 219}, {0, 114, 189}, {80, 183, 189}, {128, 128, 0}};

void draw_objs(const cv::Mat& image,const std::vector<Object>& objs,const std::vector<STrack> output_stracks,int frame,BYTETracker* tracker)
{
	for (int i = 0; i < output_stracks.size(); i++)
	{
				std::vector<float> tlwh = output_stracks[i].tlwh;
				bool vertical = tlwh[2] / tlwh[3] > 1.6;
				
				if (tlwh[2] * tlwh[3] > 20 && !vertical)
				{
					cv::Scalar s = tracker->get_color(output_stracks[i].track_id);
					 char text[256];
                	sprintf(text, "%d %s %.2f%%",output_stracks[i].track_id , CLASS_NAMES[objs[i].label].c_str(), objs[i].prob * 100);
					// fprintf(stderr, "%s = %.4f at %f %f %lf x %lf\n", CLASS_NAMES[objs[i].label].c_str(), objs[i].prob,
					// 		objs[i].rect.x, objs[i].rect.y, objs[i].rect.width, objs[i].rect.height);
					cv::putText(image, text, cv::Point(tlwh[0], tlwh[1] - 5),0, 0.6, cv::Scalar(0, 0, 255), 2, LINE_AA);

					cv::rectangle(image, cv::Rect(tlwh[0], tlwh[1], tlwh[2], tlwh[3]), s, 2);

					// cout << "is activated " << output_stracks[i].is_activated << endl;
					// cout << "Track ID " << output_stracks[i].track_id << endl;
					// cout << "State " << output_stracks[i].state << endl;
					// cout << "Frame " << output_stracks[i].frame_id << endl;
					// cout << "Tracklet Len " << output_stracks[i].tracklet_len << endl;
					// cout << "Start Frame " << output_stracks[i].start_frame << endl;
					// cout << "Score " << output_stracks[i].score << endl;
					
				}
				
	}
}




int main(int argc, char **argv)
{
	// auto start_ = std::chrono::system_clock::now();
	// system("yolo track tracker=/home/thinhdo/Study/Machine-Learning-Basic/c_13_Computer_Vision/3.C++/yolov8_ByteTrack_TensorRT/config_bytetrack.yaml model=/home/thinhdo/Study/Machine-Learning-Basic/c_13_Computer_Vision/3.C++/yolov8_ByteTrack_TensorRT/model/best.pt source=/home/thinhdo/Study/Machine-Learning-Basic/c_13_Computer_Vision/3.C++/yolov8_ByteTrack_TensorRT/data/test.mp4 imgsz=640 conf=0.25 iou=0.65 show save=true device=cpu");
	// auto end_ = std::chrono::system_clock::now();

	// printf(" time cost %2.4lf ms\n", chrono::duration_cast<chrono::microseconds>(end_ - start_).count() / 1000.);
	
	
	int deviceCount;
	cudaGetDeviceCount(&deviceCount);
	std::cout << deviceCount << std::endl;
	if (deviceCount == 0)
	{
		std::cout << "Không tìm thấy GPU hỗ trợ CUDA trên hệ thống!" << std::endl;
		return 0;
	}
	cudaSetDevice(0);
	std::string engine_file_path;

	engine_file_path = "../model/best.engine";

	const std::string path{argv[1]};
	std::cout << "path= " << path << std::endl;
	std::vector<std::string> imagePathList;
	bool isVideo{false};

	assert(argc == 3);
	
	if (IsFile(path))
	{
		std::string suffix = path.substr(path.find_last_of('.') + 1);
		if (
			suffix == "jpg" ||
			suffix == "jpeg" ||
			suffix == "png")
		{
			imagePathList.push_back(path);
		}
		else if (
			suffix == "mp4" ||
			suffix == "avi" ||
			suffix == "m4v" ||
			suffix == "mpeg" ||
			suffix == "mov" ||
			suffix == "mkv")
		{
			isVideo = true;
		}
		else
		{
			printf("suffix %s is wrong !!!\n", suffix.c_str());
			std::abort();
		}
	}
	else if (IsFolder(path))
	{
		cv::glob(path + "/*.jpg", imagePathList);
	}
	auto start1 = std::chrono::system_clock::now();
	auto start=std::chrono::system_clock::now();
	// std::cout<<"path:"<<path<<std::endl;
	auto yolov8 = new YOLOv8(engine_file_path);

	auto tracker = new BYTETracker(25, 25);

	yolov8->make_pipe(true);

	cv::Mat res, image;
	cv::Size size = cv::Size{640, 640};
	std::vector<Object> objs;
	int num_frames = 0;
	int total_ms = 0;

	cv::namedWindow("result", cv::WINDOW_AUTOSIZE);

	if (isVideo)
	{
		cv::VideoCapture cap(path);
		if (!cap.isOpened())
		{
			printf("Error opening video file %s\n", path.c_str());
			return -1;
		}

		
		cv::VideoWriter video("../data/video_output/output1.mp4", cv::VideoWriter::fourcc('m', 'p', '4', 'v'), 10, size);
		
		while (cap.read(image))
		{
			num_frames++;
			
			
			objs.clear();
			// cv::Mat image_=image;
			cv::resize(image,image,size,cv::INTER_LINEAR);
			yolov8->copy_from_Mat(image, size);
			auto start = std::chrono::system_clock::now();
			yolov8->infer();
			auto end = std::chrono::system_clock::now();

			// printf("infer time cost %2.4lf ms\n", chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.);

			yolov8->postprocess(objs);

			auto trackerstart = std::chrono::system_clock::now();
			vector<STrack> output_stracks = tracker->update(objs);

			auto trackerend = std::chrono::system_clock::now();

			// printf("tracker cost %2.4lf ms\n", chrono::duration_cast<chrono::microseconds>(trackerend - trackerstart).count() / 1000.);
			draw_objs(image,objs,output_stracks,num_frames,tracker);

			
			total_ms = total_ms + chrono::duration_cast<chrono::microseconds>(trackerend - start).count();

			cv::putText(image, cv::format("frame: %d fps: %d num: %d", num_frames, num_frames * 1000000 / total_ms, output_stracks.size()),
							cv::Point(0, 30), 0, 0.6, cv::Scalar(0, 0, 255), 2, LINE_AA);

			auto allend = std::chrono::system_clock::now();

			auto tc = (double)std::chrono::duration_cast<std::chrono::microseconds>(allend - start).count() /1000.;
			// printf("all time cost %2.4lf ms\n", tc);
			std::string text="../data/image/1image_"+std::to_string(num_frames)+".png";
			cv::imwrite(text, image);
			cv::imshow("result", image);
			video.write(image);
			if (cv::waitKey(10) == 'q')
			{
				break;
			}
		}
		video.release();

	}
	else
	{
		for (auto &path : imagePathList)
		{
			objs.clear();
			image = cv::imread(path);
			yolov8->copy_from_Mat(image, size);
			auto start = std::chrono::system_clock::now();
			yolov8->infer();
			auto end = std::chrono::system_clock::now();
			yolov8->postprocess(objs);
			auto trackerstart = std::chrono::system_clock::now();
			vector<STrack> output_stracks = tracker->update(objs);
			auto trackerend = std::chrono::system_clock::now();
			for (int i = 0; i < output_stracks.size(); i++)
			{
				vector<float> tlwh = output_stracks[i].tlwh;
				bool vertical = tlwh[2] / tlwh[3] > 1.6;
				if (tlwh[2] * tlwh[3] > 20 && !vertical)
				{
					int s = output_stracks[i].track_id;
					cout << " ID " << output_stracks[i].track_id << endl;
					yolov8->draw_objects(image, res, objs, CLASS_NAMES, COLORS, output_stracks[i].track_id);
					cout << " ID is " << output_stracks[i].track_id << endl;
				}
			}
			yolov8->draw_objects(image, res, objs, CLASS_NAMES, COLORS, 0);
			// yolov8->draw_objects(image, res, objs, CLASS_NAMES, COLORS);
			auto tc = (double)
						  std::chrono::duration_cast<std::chrono::microseconds>(end - start)
							  .count() /
					  1000.;
			printf("Detection cost %2.4lf ms\n", tc);
			auto tt = (double)
						  std::chrono::duration_cast<std::chrono::microseconds>(trackerend - trackerstart)
							  .count() /
					  1000.;
			printf("Tracker cost %2.4lf ms\n", tt);

			cv::imshow("result", image);
			// video.write(image);
			cv::imwrite("../data/image/result.png", image);
			//			cv::waitKey(1000/15);
		}
	}
	cv::destroyAllWindows();
	delete yolov8;
	return 0;
}
