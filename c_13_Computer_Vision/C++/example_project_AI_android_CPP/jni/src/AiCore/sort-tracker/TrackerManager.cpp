

/*
    Module: Object Tracking
    Author: Le Manh Thang
    Created: August 26th, 2021
*/
#include "TrackerManager.hpp"
#include <iostream>
#include <fstream>
#include <iomanip> // to format image names using setw() and setfill()
//#include <io.h>    // to check file existence using POSIX function access(). On Linux 
#include <unistd.h>
#include <set>


TrackerManager::TrackerManager(Rect2f rect, string typeObject)
{
    this->tracker = KalmanTracker(rect);
    this->tracer.m_type = typeObject;
    this->tracer.isOutOfFrame = false;
}

TrackerManager::~TrackerManager()
{

}

void TrackerManager::update(Rect2f rect)
{   
    this->tracker.update(rect);
    this->tracer.isOutOfFrame = false;
    this->tracer.m_rect = rect;
    this->tracer.m_ID = this->tracker.m_id + 1;
    this->tracer.m_trace.push_back(this->tracker.get_point());
    if (this->tracer.m_trace.size() > max_trace_length)
    {
        this->tracer.m_trace.pop_front(this->tracer.m_trace.size() - max_trace_length);
    }
}

void TrackerManager::update_scale(Rect2f rect, Rect2f rect_scale)
{   
    this->tracker.update(rect_scale);
    this->tracer.isOutOfFrame = false;
    this->tracer.m_rect = rect;
    this->tracer.m_rect_scale = rect_scale;
    this->tracer.m_ID = this->tracker.m_id + 1;
    this->tracer.m_trace.push_back(this->tracker.get_point());
    if (this->tracer.m_trace.size() > max_trace_length)
    {
        this->tracer.m_trace.pop_front(this->tracer.m_trace.size() - max_trace_length);
    }
}

TrackingTrace TrackerManager::get() const
{   
    return this->tracer;
}

Rect2f TrackerManager::predict()
{
    return this->tracker.predict();
}

bool TrackerManager::isConfirmed()
{
    if(this->tracker.m_time_since_update == 0)
        this->tracer.isOutOfFrame = false;
    else 
        this->tracer.isOutOfFrame = true;
        
    if(this->tracker.m_hit_streak >=  min_confirmed && this->tracker.m_time_since_update <= max_age)
    {
        
        return true;
    }
    return false;
}

bool TrackerManager::isConfirmed_fr()
{
    if(this->tracker.m_time_since_update == 0)
        this->tracer.isOutOfFrame = false;
    else 
        this->tracer.isOutOfFrame = true;
        
    if(this->tracker.m_hit_streak >=  min_confirmed_fr && this->tracker.m_time_since_update <= max_age)
    {
        
        return true;
    }
    return false;
}

bool TrackerManager::isDeleted()
{
    this->tracer.isOutOfFrame = true;
    return (this->tracker.m_time_since_update > max_age);
}

bool TrackerManager::isTentative()
{
    return (this->tracker.m_time_since_update > 1);
}

bool TrackerManager::isOutOfFrame()
{
    this->tracer.isOutOfFrame = true;
    return (this->tracker.m_time_since_update > 0);
}

TrackerManager::TrackerManager(cv::Rect2f rect, cv::Point2f landmark[5])//face tracking
{
    this->tracker = KalmanTracker(rect);
    this->tracer.m_landmark[0] = landmark[0];
    this->tracer.m_landmark[1] = landmark[1];
    this->tracer.m_landmark[2] = landmark[2];
    this->tracer.m_landmark[3] = landmark[3];
    this->tracer.m_landmark[4] = landmark[4];
    this->tracer.isOutOfFrame = false;
}

void TrackerManager::update(cv::Rect2f rect, cv::Point2f landmark[5], bool quality, bool rgbcheck)//face tracking
{
    this->tracker.update(rect);
    this->tracer.isOutOfFrame = false;
    this->tracer.m_rect = rect;
    this->tracer.m_landmark[0] = landmark[0];
    this->tracer.m_landmark[1] = landmark[1];
    this->tracer.m_landmark[2] = landmark[2];
    this->tracer.m_landmark[3] = landmark[3];
    this->tracer.m_landmark[4] = landmark[4];
    this->tracer.m_quality = quality;
    this->tracer.m_rgbcheck = rgbcheck;
    this->tracer.m_ID = this->tracker.m_id + 1;
    this->tracer.m_trace.push_back(this->tracker.get_point());
    if(this->tracer.m_trace.size() > max_trace_length) {
        this->tracer.m_trace.pop_front(this->tracer.m_trace.size() - max_trace_length);
    }
}

