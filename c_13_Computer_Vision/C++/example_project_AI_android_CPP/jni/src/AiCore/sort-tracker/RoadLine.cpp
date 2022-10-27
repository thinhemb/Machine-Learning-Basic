#include "RoadLine.hpp"

RoadLine::RoadLine() {

}

RoadLine::~RoadLine() {

}

RoadLine::RoadLine(const cv::Point2f& pt1, const cv::Point2f& pt2, unsigned int uid)
    :
        m_pt1(pt1), m_pt2(pt2), m_uid(uid) {
}

bool RoadLine::operator==(const RoadLine &line) {
    return line.m_uid == m_uid;
}

void RoadLine::Draw(cv::Mat frame) {
    auto Ptf2i = [&](cv::Point2f pt) -> cv::Point {
        return cv::Point(cvRound(frame.cols * pt.x), cvRound(frame.rows * pt.y));
    };
    cv::line(frame, Ptf2i(m_pt1), Ptf2i(m_pt2), cv::Scalar(0, 255, 255), 1, cv::LINE_8, 0);
    std::string label = "Line " + std::to_string(m_uid) + ": " + std::to_string(m_intersect1) + "/" + std::to_string(m_intersect2);
    int baseLine = 0;
    double fontScale = 0.7;
    int thickness = 1;
    cv::Size labelSize = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, fontScale, 1, &baseLine);
    cv::Point pt(Ptf2i(0.5f * (m_pt1 + m_pt2)));
    pt.y += labelSize.height;
    //pt.x += labelSize.width;
    cv::putText(frame, label, pt, cv::FONT_HERSHEY_TRIPLEX, fontScale, cv::Scalar(0, 0, 0), thickness);       
}

int RoadLine::IsIntersect(cv::Point2f pt1, cv::Point2f pt2) {
    bool isIntersect = CheckIntersection(pt1, pt2);
    int direction = 0;

    if (isIntersect)
    {
        cv::Point2f pt;
        if ((m_pt1.x <= m_pt2.x) && (m_pt1.y > m_pt2.y))
        {
            pt.x = (m_pt1.x + m_pt2.x) / 2.f - 0.01f;
            pt.y = (m_pt1.y + m_pt2.y) / 2.f - 0.01f;
        }
        else
        {
            if ((m_pt1.x <= m_pt2.x) && (m_pt1.y <= m_pt2.y))
            {
                pt.x = (m_pt1.x + m_pt2.x) / 2.f + 0.01f;
                pt.y = (m_pt1.y + m_pt2.y) / 2.f - 0.01f;
            }
            else
            {
                if ((m_pt1.x > m_pt2.x) && (m_pt1.y > m_pt2.y))
                {
                    pt.x = (m_pt1.x + m_pt2.x) / 2.f - 0.01f;
                    pt.y = (m_pt1.y + m_pt2.y) / 2.f + 0.01f;
                }
                else
                {
                    if ((m_pt1.x > m_pt2.x) && (m_pt1.y <= m_pt2.y))
                    {
                        pt.x = (m_pt1.x + m_pt2.x) / 2.f + 0.01f;
                        pt.y = (m_pt1.y + m_pt2.y) / 2.f + 0.01f;
                    }
                }
            }
        }
        if (CheckIntersection(pt1, pt))
        {
            direction = 1;
            ++m_intersect1;
        }
        else
        {
            direction = 2;
            ++m_intersect2;
        }
    }

    return direction;
}

bool RoadLine::CheckIntersection(cv::Point2f pt1, cv::Point2f pt2) {
    const float eps = 0.00001f; // Epsilon for equal comparing

    // First line equation
    float a1 = 0;
    float b1 = 0;
    bool trivial1 = false; // Is first line is perpendicular with OX

    if (fabs(m_pt1.x - m_pt2.x) < eps) {
        trivial1 = true;
    }
    else {
        a1 = (m_pt2.y - m_pt1.y) / (m_pt2.x - m_pt1.x);
        b1 = (m_pt2.x * m_pt1.y - m_pt1.x * m_pt2.y) / (m_pt2.x - m_pt1.x);
    }

    // Second line equation
    float a2 = 0;
    float b2 = 0;
    bool trivial2 = false; // Is second line is perpendicular with OX

    if (fabs(pt1.x - pt2.x) < eps) {
        trivial2 = true;
    }
    else {
        a2 = (pt2.y - pt1.y) / (pt2.x - pt1.x);
        b2 = (pt2.x * pt1.y - pt1.x * pt2.y) / (pt2.x - pt1.x);
    }

    // Intersection coords
    cv::Point2f intersectPt;

    bool isIntersect = true;
    if (trivial1) {
        if (trivial2)
            isIntersect = (fabs(m_pt1.x - pt1.x) < eps);
        else
            intersectPt.x = m_pt1.x;

        intersectPt.y = a2 * intersectPt.x + b2;
    }
    else {
        if (trivial2) {
            intersectPt.x = pt1.x;
        }
        else {
            if (fabs(a2 - a1) > eps)
                intersectPt.x = (b1 - b2) / (a2 - a1);
            else
                isIntersect = false;
        }
        intersectPt.y = a1 * intersectPt.x + b1;
    }

    if (isIntersect) {
        auto InRange = [](float val, float minVal, float  maxVal) -> bool {
            return (val >= minVal) && (val <= maxVal);
        };

        isIntersect = InRange(intersectPt.x, std::min(m_pt1.x, m_pt2.x) - eps, std::max(m_pt1.x, m_pt2.x) + eps) &&
                InRange(intersectPt.x, std::min(pt1.x, pt2.x) - eps, std::max(pt1.x, pt2.x) + eps) &&
                InRange(intersectPt.y, std::min(m_pt1.y, m_pt2.y) - eps, std::max(m_pt1.y, m_pt2.y) + eps) &&
                InRange(intersectPt.y, std::min(pt1.y, pt2.y) - eps, std::max(pt1.y, pt2.y) + eps);
    }
    return isIntersect;
}