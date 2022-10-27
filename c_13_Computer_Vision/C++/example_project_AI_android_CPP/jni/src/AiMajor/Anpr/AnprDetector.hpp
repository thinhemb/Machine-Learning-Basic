/*
    Module: AnprDetector.cpp
    Author: ThangLmb
    Date: June 24, 2021
*/
#ifndef AnprDetector_hpp
#define AnprDetector_hpp
#define VEHICLE_DETECTION 0
#include "ObjectDetector.hpp"
#include "VehicleDetector.hpp"

#define THRES_DETECT 0.35
enum class Nations {VN, VnSquare, VnRect, US, MALAY, China};

class AnprDetector
{
private:
    /* data */
    ObjectDetector* obj_detector = nullptr;
    #if(VEHICLE_DETECTION == 1)
        VehicleDetector* vehicle = nullptr;
    #endif
public:
    AnprDetector(/* args */);
    ~AnprDetector();
    int init(Nations nation);
    int detect ( cv::Mat& img, std::vector<ObjectTrace>& objects);
    
};

#endif