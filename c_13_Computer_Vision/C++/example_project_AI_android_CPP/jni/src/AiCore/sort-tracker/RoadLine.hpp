#pragma once
#include <iostream>
#include <vector>
#include "opencv2/opencv.hpp"

class RoadLine {
public:
    RoadLine();
    RoadLine(const cv::Point2f& pt1, const cv::Point2f& pt2, unsigned int uid);
    ~RoadLine();
    cv::Point2f m_pt1;
    cv::Point2f m_pt2;
    unsigned int m_uid = 0;

    int m_intersect1 = 0;
    int m_intersect2 = 0;

    bool operator==(const RoadLine &line);
    void Draw(cv::Mat frame);
    int IsIntersect(cv::Point2f pt1, cv::Point2f pt2);

private:
    bool CheckIntersection(cv::Point2f pt1, cv::Point2f pt2);     
};