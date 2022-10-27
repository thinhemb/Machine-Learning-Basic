#include <opencv2/opencv.hpp>
#include "ObjectTracking.hpp"

#define INF 10000
// Given three colinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
inline bool onSegment(cv::Point p, cv::Point q, cv::Point r)
{
    if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
            q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
        return true;
    return false;
}
 
// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
inline int orientation(cv::Point p, cv::Point q, cv::Point r)
{
    int val = (q.y - p.y) * (r.x - q.x) -
            (q.x - p.x) * (r.y - q.y);
 
    if (val == 0) return 0; // colinear
    return (val > 0)? 1: 2; // clock or counterclock wise
}
 
// The function that returns true if line segment 'p1q1'
// and 'p2q2' intersect.
inline bool doIntersect(cv::Point p1, cv::Point q1, cv::Point p2, cv::Point q2)
{
    // Find the four orientations needed for general and
    // special cases
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);
 
    // General case
    if (o1 != o2 && o3 != o4)
        return true;
 
    // Special Cases
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;
 
    // p1, q1 and p2 are colinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;
 
    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;
 
    // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;
 
    return false; // Doesn't fall in any of the above cases
}


ObjectTracking::ObjectTracking()
{
    this->m_tracker = std::make_unique<ObjectTracker>();
}

ObjectTracking::~ObjectTracking() 
{
  
}

void ObjectTracking::process( vector<ObjectTrace> detBoxes, std::vector<TrackingTrace>& tracks) 
{
    // double start = getTickCount();
    this->m_tracker->update(detBoxes);
    tracks = this->m_tracker->getTracks();
    // double time = (getTickCount() - start) / getTickFrequency();
}

void ObjectTracking::process( vector<ObjectTrace> detBoxes, std::vector<TrackingTrace>& tracks, float scale)
{
    this->m_tracker->update_scale(detBoxes, scale);
    tracks = this->m_tracker->getTracks();
}

void ObjectTracking::process_wh( vector<ObjectTrace> detBoxes, std::vector<TrackingTrace>& tracks, float scale_width, float scale_height)
{
    this->m_tracker->update_scale_wh(detBoxes, scale_width, scale_height);
    tracks = this->m_tracker->getTracks();
}

void ObjectTracking::process_anpr_VN( vector<ObjectTrace> detBoxes, std::vector<TrackingTrace>& tracks, 
                                    float scale_w_rect, float scale_h_rect, float scale_w_square, float scale_h_square)
{
    this->m_tracker->update_scale_wh_anpr(detBoxes, scale_w_rect, scale_h_rect, scale_w_square, scale_h_square);
    tracks = this->m_tracker->getTracks();
}

// void ObjectTracking::process( vector<FaceObject> detBoxes, std::vector<TrackingTrace>& tracks) //face tracking
// {
//     this->m_tracker->update(detBoxes);
//     tracks = this->m_tracker->getTracks_fr();
// }

void ObjectTracking::AddLine(const RoadLine& newLine)
{
    m_lines.push_back(newLine);
}

bool ObjectTracking::GetLine(unsigned int lineUid, RoadLine& line)
{
    for (const auto& rl : m_lines)
    {
        if (rl.m_uid == lineUid)
        {
            line = rl;
            return true;
        }
    }
    return false;
}

bool ObjectTracking::RemoveLine(unsigned int lineUid)
{
    for (auto it = std::begin(m_lines); it != std::end(m_lines);)
    {
        if (it->m_uid == lineUid)
            it = m_lines.erase(it);
        else
            ++it;
    }
    return false;
}



bool ObjectTracking::checkIntersection(const TrackingTrace& track, float xMax, float yMax, RoadLine line, std::string type) {
    auto Pti2f = [&](cv::Point pt) -> cv::Point2f {
        return cv::Point2f(pt.x / xMax, pt.y / yMax);
    };
    constexpr size_t minTrack = 5;
    if (track.m_trace.size() >= minTrack) {
        int direction = line.IsIntersect(Pti2f(track.m_trace[track.m_trace.size() - minTrack]), Pti2f(track.m_trace[track.m_trace.size() - 1]));
        if (type == "DEF" && direction > 0) {
            return true;
        }
        else if (type == "IN" && direction == 2) {
            return true;
        }
        else if (type == "OUT" && direction == 1) {
            return true;
        }
    }
    return false;
}

// int ObjectTracking::checkCrossline(const TrackingTrace& track, float xMax, float yMax, RoadLine line) 
// {
//     auto Pti2f = [&](cv::Point pt) -> cv::Point2f {
//         return cv::Point2f(pt.x / xMax, pt.y / yMax);
//     };
//     constexpr size_t minTrack = 5;  
//     int direction  = 0;
//     if (track.m_trace.size() >= minTrack) {
//         if (m_lastIntersections.find(track.m_ID) == m_lastIntersections.end()) {
//             direction = line.IsIntersect(Pti2f(track.m_trace[track.m_trace.size() - minTrack]), Pti2f(track.m_trace[track.m_trace.size() - 1]));
//             if(direction > 0)
//                 m_lastIntersections.emplace(track.m_ID);
//         }
//     }
//     if(m_lastIntersections.size() > 30) m_lastIntersections.clear();
//     return direction;
// }
int ObjectTracking::checkCrossline(const TrackingTrace& track, float xMax, float yMax, RoadLine line) 
{
    auto Pti2f = [&](cv::Point pt) -> cv::Point2f {
        return cv::Point2f(pt.x / xMax, pt.y / yMax);
    };
    constexpr size_t minTrack = 5;  
    int direction  = 0;
    if (track.m_trace.size() >= minTrack) {
            direction = line.IsIntersect(Pti2f(track.m_trace[track.m_trace.size() - minTrack]), Pti2f(track.m_trace[track.m_trace.size() - 1]));
    }
    return direction;
}
int ObjectTracking::eraseCrossed(const TrackingTrace& track)
{
    if (m_lastIntersections.find(track.m_ID) != m_lastIntersections.end())
        m_lastIntersections.erase(track.m_ID);
    return 0;
}


bool ObjectTracking::isInsideRegion(std::vector<cv::Point> polygon, cv::Point p)
{
    // There must be at least 3 vertices in polygon[]
    if (polygon.size() < 3) return false;
 
    // Create a point for line segment from p to infinite
    cv::Point extreme = {INF, p.y};
 
    // Count intersections of the above line with sides of polygon
    int count = 0, i = 0;
    do
    {
        int next = (i+1)%polygon.size();
 
        // Check if the line segment from 'p' to 'extreme' intersects
        // with the line segment from 'polygon[i]' to 'polygon[next]'
        if (doIntersect(polygon[i], polygon[next], p, extreme))
        {
            // If the point 'p' is colinear with line segment 'i-next',
            // then check if it lies on segment. If it lies, return true,
            // otherwise false
            if (orientation(polygon[i], p, polygon[next]) == 0)
            return onSegment(polygon[i], p, polygon[next]);
 
            count++;
        }
        i = next;
    } while (i != 0);
 
    // Return true if count is odd, false otherwise
    return count&1; // Same as (count%2 == 1)
}
