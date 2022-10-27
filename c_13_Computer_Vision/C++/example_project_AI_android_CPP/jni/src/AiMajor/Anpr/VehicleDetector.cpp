/*
    Module: VehicleDetector.cpp
    Author: ThangLmb
    Date: June 24, 2021
*/
#include "VehicleDetector.hpp"

VehicleDetector::VehicleDetector(/* args */)
{
    this->vehicle_detector = new ObjectDetector();
    //this->vehicle_detector->initObjectDetector("ncnn", "traffic");
    this->vehicle_detector->initObjectDetector("snpe", "traffic");
}

VehicleDetector::~VehicleDetector()
{
    if(this->vehicle_detector != nullptr) delete this->vehicle_detector;
}

int VehicleDetector::detect (const cv::Mat& img, std::vector<ObjectTrace>& objects)
{
    objects.clear();
    this->vehicle_detector->executeObjectDetector(img, objects, 0.3);
    return 0;
}