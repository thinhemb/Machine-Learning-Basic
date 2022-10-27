#ifndef OBJECTTRACKING_HPP
#define OBJECTTRACKING_HPP

#include "ObjectTracker.hpp"
#include "RoadLine.hpp"
// #include "../AiCore/face_detection/retinaface/FaceDetector.hpp"
class ObjectTracking {
public:
    ObjectTracking();
    virtual ~ObjectTracking();
    
    void process( vector<ObjectTrace> detBoxes, std::vector<TrackingTrace>& tracks);
    void process( vector<ObjectTrace> detBoxes, std::vector<TrackingTrace>& tracks, float scale);
    void process_wh( vector<ObjectTrace> detBoxes, std::vector<TrackingTrace>& tracks, float scale_width, float scale_height);
    void process_anpr_VN( vector<ObjectTrace> detBoxes, std::vector<TrackingTrace>& tracks, 
                        float scale_w_rect, float scale_h_rect, float scale_w_square, float scale_h_square);
    // void process( vector<FaceObject> detBoxes, std::vector<TrackingTrace>& tracks);

    // Lines API
    void AddLine(const RoadLine& newLine);
    bool GetLine(unsigned int li6neUid, RoadLine& line);
    bool RemoveLine(unsigned int lineUid);
    std::deque<RoadLine> m_lines;
    void CheckLinesIntersection(const TrackingTrace& track, float xMax, float yMax, std::string type);
    bool checkIntersection(const TrackingTrace& track, float xMax, float yMax, RoadLine line, std::string type = "DEF");
    int checkCrossline(const TrackingTrace& track, float xMax, float yMax, RoadLine line);
    int eraseCrossed(const TrackingTrace& track);
    bool isInsideRegion(std::vector<cv::Point> polygon, cv::Point p);
    std::set<size_t> m_lastIntersections;
    void DrawTrack(cv::Mat frame,
                    int resizeCoeff,
                    const TrackingTrace& track,
                    bool drawTrajectory);
private:
    float fps = 15;
    std::vector<cv::Scalar> colors;
    std::unique_ptr<ObjectTracker> m_tracker;
};

#endif /* OBJECTTRACKING_HPP */