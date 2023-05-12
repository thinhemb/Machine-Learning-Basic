/******************************************************************************** 
 Module: SnpeRuntime.cpp
 Author: LE MANH THANG
 Created: 08/02/2021
 Description: 
********************************************************************************/
#ifndef  SNPEDETECTOR_H
#define SNPEDETECTOR_H
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "SnpeMobilenetSSD.hpp"

class SnpeDetector : SnpeMobilenetSSD
{
private:
    /* data */
    SnpeMobilenetSSD* detector;
public:
    SnpeDetector(/* args */);
    ~SnpeDetector();
    int initSnpeObjectDetector(std::string pathModelDlc);
    int initSnpeObjectDetector(std::string pathModelDlc, std::string targetDevice);
    int executeSnpeObjectDetector(const cv::Mat& img, std::vector<std::string>& labels, std::vector<ObjectTrace>& objects, float thres_detect);
};




#endif