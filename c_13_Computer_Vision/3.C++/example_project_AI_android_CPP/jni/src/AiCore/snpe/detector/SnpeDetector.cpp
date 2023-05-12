/******************************************************************************** 
 Module: SnpeRuntime.cpp
 Author: LE MANH THANG
 Created: 08/02/2021
 Description: 
********************************************************************************/
#include "SnpeDetector.hpp"

SnpeDetector::SnpeDetector(/* args */)
{
    this->detector = new SnpeMobilenetSSD();
}

SnpeDetector::~SnpeDetector()
{
    if(this->detector) delete this->detector;
}

int SnpeDetector::initSnpeObjectDetector(std::string pathModelDlc)
{
    int net = this->detector->initSnpeMobilenetSSD(pathModelDlc);
    if( net != 0)
    {
        LOG_FAIL("Init Mobilenet SSD failed");
        return -1;
    } 
    return 0;
}


int SnpeDetector::initSnpeObjectDetector(std::string pathModelDlc, std::string targetDevice)
{
    int net = this->detector->initSnpeMobilenetSSD(pathModelDlc, targetDevice);
    if( net != 0)
    {
        LOG_FAIL("Init Mobilenet SSD failed");
        return -1;
    } 
    return 0;
}



int SnpeDetector::executeSnpeObjectDetector(const cv::Mat& img, std::vector<std::string>& labels, std::vector<ObjectTrace>& objects, float thres_detect)
{
    int net = this->detector->executeSnpeMobilenetSSD(img, labels, objects, thres_detect);
    if(net != 0)
    {
        LOG_FAIL("Execute Mobilenet SSD for object detection failed");
    }
    return 0;
}