#ifndef STRUCT_HPP
#define STRUCT_HPP

#include <iostream>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

struct Object
{
    int frame;
	int id;
	int obj_id;
    cv::Rect_<float> rect;
    std::string label;
    float prob;
};

typedef struct TrackingBox
{
	int frame;
	int id;
    std::string label;
    int obj_id;
    float score;
	cv::Rect_<float> box;
    
    
}TrackingBox;

struct BoxInfo 
{
    int label;
    float score;
    float x1;
    float y1;
    float x2;
    float y2;
};

struct YoloObj
{
    float x;
    float y;
    float w;
    float h;
    int label;
    float prob;
};

#endif