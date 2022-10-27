/*
    Module: VehicleDetector.cpp
    Author: ThangLmb
    Date: June 24, 2021
*/
#ifndef VehicleDetector_hpp
#define VehicleDetector_hpp

#include "ObjectDetector.hpp"

class VehicleDetector
{
private:
    /* data */
    ObjectDetector* vehicle_detector = nullptr;
public:
    VehicleDetector(/* args */);
    ~VehicleDetector();
    int detect (const cv::Mat& img, std::vector<ObjectTrace>& objects);
};


#endif