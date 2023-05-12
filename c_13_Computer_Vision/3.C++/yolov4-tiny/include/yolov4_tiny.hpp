#ifndef DETECT_HPP
#define DETECT_HPP

#include <iostream>
#include <vector>
#include <regex>
#include <fstream>
#include <stdio.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/videoio/videoio_c.h>

#include "net.h"

#include "struct.hpp"

class NcnnDetect
{
    private:
        ncnn::Net* net = nullptr;
        ncnn::UnlockedPoolAllocator* g_blob_pool_allocator_detect = nullptr;
        //Mở khóa sau khi chuỗi vận hành tài nguyên
        ncnn::PoolAllocator* g_workspace_pool_allocator_detect = nullptr; 
        //Trình cấp phát bộ nhớ tùy chỉnh trong C ++ để cải thiện hiệu suất của cấp phát bộ nhớ động
        int width_model;
        int height_model;
        // size image(input)
    public:
        NcnnDetect();
        ~NcnnDetect();
        int initNcnnDetect(const char* model_bin, const char* model_param);
        int runNcnnDetect(const cv::Mat& img, std::vector<std::string> &labels, std::vector<TrackingBox> &objects, float threshold, int frame);
};

#endif