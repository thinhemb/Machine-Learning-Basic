
/*
    Module: Object Tracking
    Author: Le Manh Thang
    Created: August 26th, 2021
*/

#include "ObjectTracker.hpp"

inline double iou(Rect2f bb_test, Rect2f bb_gt)
{
	float in = (bb_test & bb_gt).area();
	float un = bb_test.area() + bb_gt.area() - in;

	if (un < DBL_EPSILON)
		return 0;

	return (double)(in / un);
}

inline std::vector<std::vector<double>> iouCal(const vector<Rect2f> &dets, const vector<Rect2f> &trks) 
{
    auto trk_num = trks.size();
    auto det_num = dets.size();
     // using the fill constructor
    std::vector<std::vector<double>> dist;
    dist.clear();
    dist.resize(trk_num, vector<double>(det_num, 0));
    for (size_t i = 0; i < trk_num; i++) // compute iou matrix as a distance matrix
    {
        for (size_t j = 0; j < det_num; j++) 
        {
            dist[i][j] = 1 - iou(trks[i], dets[j]);
        }
    }
    return dist;
}

ObjectTracker::ObjectTracker(/* args */)
{
    
}

ObjectTracker::~ObjectTracker()
{

}

void ObjectTracker::update(vector<ObjectTrace> detected)
{   
    vector<Rect_<float>> detectedBox;
    for(auto det : detected) detectedBox.push_back(det.rect);
	vector<Rect_<float>> predictedBoxes;
	vector<vector<double>> iouMatrix;
	vector<int> assignment;
	set<int> unmatchedDetections;
	set<int> unmatchedTrajectories;
	set<int> allItems;
	set<int> matchedItems;
	vector<cv::Point> matchedPairs;
	unsigned int trkNum = 0;
	unsigned int detNum = 0;

    predictedBoxes.clear();
    for (auto it = this->trackers.begin(); it != this->trackers.end();)
    {
        Rect_<float> pBox = (*it)->predict();
        if (pBox.x >= 0 && pBox.y >= 0)
        {
            predictedBoxes.push_back(pBox);
            it++;
        }
        else
        {
            it = this->trackers.erase(it);
        }
    }
    ///////////////////////////////////////
    // 3.2. associate detections to tracked object (both represented as bounding boxes)
    // dets : detectedBox
    trkNum = predictedBoxes.size();
    detNum = detectedBox.size();
    iouMatrix = iouCal(detectedBox, predictedBoxes );

    // solve the assignment problem using hungarian algorithm.
    // the resulting assignment is [track(prediction) : detection], with len=preNum
    HungarianAlgorithm HungAlgo;
    assignment.clear();
    HungAlgo.Solve(iouMatrix, assignment);
    
    // find matches, unmatched_detections and unmatched_predictions
    unmatchedTrajectories.clear();
    unmatchedDetections.clear();
    allItems.clear();
    matchedItems.clear();

    if (detNum > trkNum) //	there are unmatched detections
    {
        for (unsigned int n = 0; n < detNum; n++)
            allItems.insert(n);

        for (unsigned int i = 0; i < trkNum; ++i)
            matchedItems.insert(assignment[i]);

        set_difference(allItems.begin(), allItems.end(),
            matchedItems.begin(), matchedItems.end(),
            insert_iterator<set<int>>(unmatchedDetections, unmatchedDetections.begin()));
    }
    else if (detNum < trkNum) // there are unmatched trajectory/predictions
    {
        for (unsigned int i = 0; i < trkNum; ++i)
            if (assignment[i] == -1) // unassigned label will be set as -1 in the assignment algorithm
                unmatchedTrajectories.insert(i);
    }
    else
        ;

    // filter out matched with low IOU
    matchedPairs.clear();
    for (unsigned int i = 0; i < trkNum; ++i)
    {
        if (assignment[i] == -1) // pass over invalid values
            continue;
        if (1 - iouMatrix[i][assignment[i]] < iouThreshold)
        {
            unmatchedTrajectories.insert(i);
            unmatchedDetections.insert(assignment[i]);
        }
        else
            matchedPairs.push_back(cv::Point(i, assignment[i]));
    }

    ///////////////////////////////////////
    // 3.3. updating this->trackers
    // update matched this->trackers with assigned detections.
    // each prediction is corresponding to a tracker
    int detIdx, trkIdx;
    for (unsigned int i = 0; i < matchedPairs.size(); i++)
    {
        trkIdx = matchedPairs[i].x;
        detIdx = matchedPairs[i].y;
        this->trackers[trkIdx]->update(detectedBox[detIdx]);
    }

    // create and initialise new this->trackers for unmatched detections
    // or update if object is out of frame and object was not deleted
    for (auto umd : unmatchedDetections)
    {
        this->trackers.push_back(std::make_unique<TrackerManager>(detectedBox[umd], detected[umd].label));
    }
}

// update with scale box
void ObjectTracker::update_scale(vector<ObjectTrace> detected, float scale)
{   
    vector<Rect_<float>> detectedBox;
    vector<Rect_<float>> detectedBoxScale; // 
    for(auto det : detected) {
        detectedBox.push_back(det.rect);
        det.rect_scale.width = det.rect.width * scale; //
        det.rect_scale.height = det.rect.height * scale; // 
        det.rect_scale.x = det.rect.x - (det.rect_scale.width - det.rect.width) / 2; // 
        det.rect_scale.y = det.rect.y - (det.rect_scale.height - det.rect.height) / 2; //
        detectedBoxScale.push_back(det.rect_scale); // 
    }
	vector<Rect_<float>> predictedBoxes;
	vector<vector<double>> iouMatrix;
	vector<int> assignment;
	set<int> unmatchedDetections;
	set<int> unmatchedTrajectories;
	set<int> allItems;
	set<int> matchedItems;
	vector<cv::Point> matchedPairs;
	unsigned int trkNum = 0;
	unsigned int detNum = 0;

    predictedBoxes.clear();
    for (auto it = this->trackers.begin(); it != this->trackers.end();)
    {
        Rect_<float> pBox = (*it)->predict();
        if (pBox.x >= 0 && pBox.y >= 0)
        {
            predictedBoxes.push_back(pBox);
            it++;
        }
        else
        {
            it = this->trackers.erase(it);
        }
    }
    ///////////////////////////////////////
    // 3.2. associate detections to tracked object (both represented as bounding boxes)
    // dets : detectedBox
    trkNum = predictedBoxes.size();
    detNum = detectedBoxScale.size(); //
    iouMatrix = iouCal(detectedBoxScale, predictedBoxes ); //

    // solve the assignment problem using hungarian algorithm.
    // the resulting assignment is [track(prediction) : detection], with len=preNum
    HungarianAlgorithm HungAlgo;
    assignment.clear();
    HungAlgo.Solve(iouMatrix, assignment);
    
    // find matches, unmatched_detections and unmatched_predictions
    unmatchedTrajectories.clear();
    unmatchedDetections.clear();
    allItems.clear();
    matchedItems.clear();

    if (detNum > trkNum) //	there are unmatched detections
    {
        for (unsigned int n = 0; n < detNum; n++)
            allItems.insert(n);

        for (unsigned int i = 0; i < trkNum; ++i)
            matchedItems.insert(assignment[i]);

        set_difference(allItems.begin(), allItems.end(),
            matchedItems.begin(), matchedItems.end(),
            insert_iterator<set<int>>(unmatchedDetections, unmatchedDetections.begin()));
    }
    else if (detNum < trkNum) // there are unmatched trajectory/predictions
    {
        for (unsigned int i = 0; i < trkNum; ++i)
            if (assignment[i] == -1) // unassigned label will be set as -1 in the assignment algorithm
                unmatchedTrajectories.insert(i);
    }
    else
        ;

    // filter out matched with low IOU
    matchedPairs.clear();
    for (unsigned int i = 0; i < trkNum; ++i)
    {
        if (assignment[i] == -1) // pass over invalid values
            continue;
        if (1 - iouMatrix[i][assignment[i]] < iouThreshold)
        {
            unmatchedTrajectories.insert(i);
            unmatchedDetections.insert(assignment[i]);
        }
        else
            matchedPairs.push_back(cv::Point(i, assignment[i]));
    }

    ///////////////////////////////////////
    // 3.3. updating this->trackers
    // update matched this->trackers with assigned detections.
    // each prediction is corresponding to a tracker
    int detIdx, trkIdx;
    for (unsigned int i = 0; i < matchedPairs.size(); i++)
    {
        trkIdx = matchedPairs[i].x;
        detIdx = matchedPairs[i].y;
        this->trackers[trkIdx]->update_scale(detectedBox[detIdx], detectedBoxScale[detIdx]); //
    }

    // create and initialise new this->trackers for unmatched detections
    // or update if object is out of frame and object was not deleted
    for (auto umd : unmatchedDetections)
    {
        this->trackers.push_back(std::make_unique<TrackerManager>(detectedBoxScale[umd], detected[umd].label)); //
    }
}

// update with scale width height
void ObjectTracker::update_scale_wh(vector<ObjectTrace> detected, float scale_width, float scale_height)
{   
    vector<Rect_<float>> detectedBox;
    vector<Rect_<float>> detectedBoxScale; // 
    for(auto det : detected) {
        detectedBox.push_back(det.rect);
        det.rect_scale.width = det.rect.width * scale_width; //
        det.rect_scale.height = det.rect.height * scale_height; // 
        det.rect_scale.x = det.rect.x - (det.rect_scale.width - det.rect.width) / 2; // 
        det.rect_scale.y = det.rect.y - (det.rect_scale.height - det.rect.height) / 2; //
        detectedBoxScale.push_back(det.rect_scale); // 
    }
	vector<Rect_<float>> predictedBoxes;
	vector<vector<double>> iouMatrix;
	vector<int> assignment;
	set<int> unmatchedDetections;
	set<int> unmatchedTrajectories;
	set<int> allItems;
	set<int> matchedItems;
	vector<cv::Point> matchedPairs;
	unsigned int trkNum = 0;
	unsigned int detNum = 0;

    predictedBoxes.clear();
    for (auto it = this->trackers.begin(); it != this->trackers.end();)
    {
        Rect_<float> pBox = (*it)->predict();
        if (pBox.x >= 0 && pBox.y >= 0)
        {
            predictedBoxes.push_back(pBox);
            it++;
        }
        else
        {
            it = this->trackers.erase(it);
        }
    }
    ///////////////////////////////////////
    // 3.2. associate detections to tracked object (both represented as bounding boxes)
    // dets : detectedBox
    trkNum = predictedBoxes.size();
    detNum = detectedBoxScale.size(); //
    iouMatrix = iouCal(detectedBoxScale, predictedBoxes ); //

    // solve the assignment problem using hungarian algorithm.
    // the resulting assignment is [track(prediction) : detection], with len=preNum
    HungarianAlgorithm HungAlgo;
    assignment.clear();
    HungAlgo.Solve(iouMatrix, assignment);
    
    // find matches, unmatched_detections and unmatched_predictions
    unmatchedTrajectories.clear();
    unmatchedDetections.clear();
    allItems.clear();
    matchedItems.clear();

    if (detNum > trkNum) //	there are unmatched detections
    {
        for (unsigned int n = 0; n < detNum; n++)
            allItems.insert(n);

        for (unsigned int i = 0; i < trkNum; ++i)
            matchedItems.insert(assignment[i]);

        set_difference(allItems.begin(), allItems.end(),
            matchedItems.begin(), matchedItems.end(),
            insert_iterator<set<int>>(unmatchedDetections, unmatchedDetections.begin()));
    }
    else if (detNum < trkNum) // there are unmatched trajectory/predictions
    {
        for (unsigned int i = 0; i < trkNum; ++i)
            if (assignment[i] == -1) // unassigned label will be set as -1 in the assignment algorithm
                unmatchedTrajectories.insert(i);
    }
    else
        ;

    // filter out matched with low IOU
    matchedPairs.clear();
    for (unsigned int i = 0; i < trkNum; ++i)
    {
        if (assignment[i] == -1) // pass over invalid values
            continue;
        if (1 - iouMatrix[i][assignment[i]] < iouThreshold)
        {
            unmatchedTrajectories.insert(i);
            unmatchedDetections.insert(assignment[i]);
        }
        else
            matchedPairs.push_back(cv::Point(i, assignment[i]));
    }

    ///////////////////////////////////////
    // 3.3. updating this->trackers
    // update matched this->trackers with assigned detections.
    // each prediction is corresponding to a tracker
    int detIdx, trkIdx;
    for (unsigned int i = 0; i < matchedPairs.size(); i++)
    {
        trkIdx = matchedPairs[i].x;
        detIdx = matchedPairs[i].y;
        this->trackers[trkIdx]->update_scale(detectedBox[detIdx], detectedBoxScale[detIdx]); //
    }

    // create and initialise new this->trackers for unmatched detections
    // or update if object is out of frame and object was not deleted
    for (auto umd : unmatchedDetections)
    {
        this->trackers.push_back(std::make_unique<TrackerManager>(detectedBoxScale[umd], detected[umd].label)); //
    }
}

void ObjectTracker::update_scale_wh_anpr(vector<ObjectTrace> detected, float scale_w_rect, float scale_h_rect, float scale_w_square, float scale_h_square)
{   
    vector<Rect_<float>> detectedBox;
    vector<Rect_<float>> detectedBoxScale; // 
    for(auto det : detected) {
        detectedBox.push_back(det.rect);
        if(det.label == "VnRect") {
            det.rect_scale.width = det.rect.width * scale_w_rect; //
            det.rect_scale.height = det.rect.height * scale_h_rect; // 
            det.rect_scale.x = det.rect.x - (det.rect_scale.width - det.rect.width) / 2; // 
            det.rect_scale.y = det.rect.y - (det.rect_scale.height - det.rect.height) / 2; //
        } else if(det.label == "VnSquare") {
            det.rect_scale.width = det.rect.width * scale_w_square; //
            det.rect_scale.height = det.rect.height * scale_h_square; // 
            det.rect_scale.x = det.rect.x - (det.rect_scale.width - det.rect.width) / 2; // 
            det.rect_scale.y = det.rect.y - (det.rect_scale.height - det.rect.height) / 2; //
        }
        detectedBoxScale.push_back(det.rect_scale); // 
    }
	vector<Rect_<float>> predictedBoxes;
	vector<vector<double>> iouMatrix;
	vector<int> assignment;
	set<int> unmatchedDetections;
	set<int> unmatchedTrajectories;
	set<int> allItems;
	set<int> matchedItems;
	vector<cv::Point> matchedPairs;
	unsigned int trkNum = 0;
	unsigned int detNum = 0;

    predictedBoxes.clear();
    for (auto it = this->trackers.begin(); it != this->trackers.end();)
    {
        Rect_<float> pBox = (*it)->predict();
        if (pBox.x >= 0 && pBox.y >= 0)
        {
            predictedBoxes.push_back(pBox);
            it++;
        }
        else
        {
            it = this->trackers.erase(it);
        }
    }
    ///////////////////////////////////////
    // 3.2. associate detections to tracked object (both represented as bounding boxes)
    // dets : detectedBox
    trkNum = predictedBoxes.size();
    detNum = detectedBoxScale.size(); //
    iouMatrix = iouCal(detectedBoxScale, predictedBoxes ); //

    // solve the assignment problem using hungarian algorithm.
    // the resulting assignment is [track(prediction) : detection], with len=preNum
    HungarianAlgorithm HungAlgo;
    assignment.clear();
    HungAlgo.Solve(iouMatrix, assignment);
    
    // find matches, unmatched_detections and unmatched_predictions
    unmatchedTrajectories.clear();
    unmatchedDetections.clear();
    allItems.clear();
    matchedItems.clear();

    if (detNum > trkNum) //	there are unmatched detections
    {
        for (unsigned int n = 0; n < detNum; n++)
            allItems.insert(n);

        for (unsigned int i = 0; i < trkNum; ++i)
            matchedItems.insert(assignment[i]);

        set_difference(allItems.begin(), allItems.end(),
            matchedItems.begin(), matchedItems.end(),
            insert_iterator<set<int>>(unmatchedDetections, unmatchedDetections.begin()));
    }
    else if (detNum < trkNum) // there are unmatched trajectory/predictions
    {
        for (unsigned int i = 0; i < trkNum; ++i)
            if (assignment[i] == -1) // unassigned label will be set as -1 in the assignment algorithm
                unmatchedTrajectories.insert(i);
    }
    else
        ;

    // filter out matched with low IOU
    matchedPairs.clear();
    for (unsigned int i = 0; i < trkNum; ++i)
    {
        if (assignment[i] == -1) // pass over invalid values
            continue;
        if (1 - iouMatrix[i][assignment[i]] < iouThreshold)
        {
            unmatchedTrajectories.insert(i);
            unmatchedDetections.insert(assignment[i]);
        }
        else
            matchedPairs.push_back(cv::Point(i, assignment[i]));
    }

    ///////////////////////////////////////
    // 3.3. updating this->trackers
    // update matched this->trackers with assigned detections.
    // each prediction is corresponding to a tracker
    int detIdx, trkIdx;
    for (unsigned int i = 0; i < matchedPairs.size(); i++)
    {
        trkIdx = matchedPairs[i].x;
        detIdx = matchedPairs[i].y;
        this->trackers[trkIdx]->update_scale(detectedBox[detIdx], detectedBoxScale[detIdx]); //
    }

    // create and initialise new this->trackers for unmatched detections
    // or update if object is out of frame and object was not deleted
    for (auto umd : unmatchedDetections)
    {
        this->trackers.push_back(std::make_unique<TrackerManager>(detectedBoxScale[umd], detected[umd].label)); //
    }
}

vector<TrackingTrace> ObjectTracker::getTracks()
{
    // remove
    for (auto it = this->trackers.begin(); it != this->trackers.end();)
    {
        // remove dead tracklet
        if ((*it)->isDeleted())
            it = this->trackers.erase(it);
        else
            it++;    
    }
    // check to delete object track
    vector<TrackingTrace> traceTracks;
    for (auto it = this->trackers.begin(); it != this->trackers.end();)
    {      
        if((*it)->isOutOfFrame())
        {
            (*it)->tracer.isOutOfFrame = true;
        }
        if((*it)->isConfirmed())
        {
            TrackingTrace trace = (*it)->get();
            traceTracks.push_back(trace);
            it++;
        }
        else
            it++;               
    }
    return traceTracks;
}

// vector<TrackingTrace> ObjectTracker::getTracks_fr()
// {
//     // remove
//     for (auto it = this->trackers.begin(); it != this->trackers.end();)
//     {
//         // remove dead tracklet
//         if ((*it)->isDeleted())
//             it = this->trackers.erase(it);
//         else
//             it++;    
//     }
//     // check to delete object track
//     vector<TrackingTrace> traceTracks;
//     for (auto it = this->trackers.begin(); it != this->trackers.end();)
//     {      
//         if((*it)->isOutOfFrame())
//         {
//             (*it)->tracer.isOutOfFrame = true;
//         }
//         if((*it)->isConfirmed_fr())
//         {
//             TrackingTrace trace = (*it)->get();
//             traceTracks.push_back(trace);
//             it++;
//         }
//         else
//             it++;               
//     }
//     return traceTracks;
// }

// void ObjectTracker::update(std::vector<FaceObject> detected)//face tracking
// {
//     std::vector<cv::Rect_<float>> detectedBox;
//     for(auto det : detected) {
//         detectedBox.push_back(det.rect);
//     }

//     std::vector<cv::Rect_<float>> predictedBoxes;
//     std::vector<std::vector<double>> iouMatrix;
//     std::vector<int> assignment;
//     std::set<int> unmatchedDetections;
//     std::set<int> unmatchedTrajectories;
//     std::set<int> allItems;
//     std::set<int> matchedItems;
//     std::vector<cv::Point> matchedPairs;
//     unsigned int trkNum = 0;
//     unsigned int detNum = 0;

//     predictedBoxes.clear();
//     for(auto it = this->trackers.begin(); it != this->trackers.end(); ) {
//         cv::Rect_<float> pBox = (*it)->predict();
//         if(pBox.x >= 0 && pBox.y >= 0) {
//             predictedBoxes.push_back(pBox);
//             it++;
//         } else {
//             it = this->trackers.erase(it);
//         }
//     }

//     trkNum = predictedBoxes.size();
//     detNum = detectedBox.size();
//     iouMatrix = iouCal(detectedBox, predictedBoxes);

//     HungarianAlgorithm HungAlgo;
//     assignment.clear();
//     HungAlgo.Solve(iouMatrix, assignment);

//     unmatchedTrajectories.clear();
//     unmatchedDetections.clear();
//     allItems.clear();
//     matchedItems.clear();

//     if(detNum > trkNum) {
//         for(unsigned int n=0; n<detNum; n++)
//             allItems.insert(n);

//         for(unsigned int i=0; i<trkNum; ++i)
//             matchedItems.insert(assignment[i]);
        
//         set_difference(allItems.begin(), allItems.end(), matchedItems.begin(), matchedItems.end(), 
//                         insert_iterator<std::set<int>>(unmatchedDetections, unmatchedDetections.begin()));
//     } else if(detNum < trkNum) {
//         for(unsigned int i=0; i<trkNum; ++i) {
//             if(assignment[i] == -1)
//                 unmatchedTrajectories.insert(i);
//         }
//     } else {

//     }

//     matchedPairs.clear();
//     for(unsigned int i=0; i<trkNum; ++i) {
//         if(assignment[i] == -1)
//             continue;
//         if(1 - iouMatrix[i][assignment[i]] < iouThreshold_fr) {
//             unmatchedTrajectories.insert(i);
//             unmatchedDetections.insert(assignment[i]);
//         } else {
//             matchedPairs.push_back(cv::Point(i, assignment[i]));
//         }
//     }

//     int detIdx, trkIdx;
//     for(unsigned int i=0; i<matchedPairs.size(); i++) {
//         trkIdx = matchedPairs[i].x;
//         detIdx = matchedPairs[i].y;
//         this->trackers[trkIdx]->update(detectedBox[detIdx], detected[detIdx].point, detected[detIdx].quality, detected[detIdx].rgbcheck);
//     }

//     for(auto umd : unmatchedDetections) {
//         this->trackers.push_back(std::make_unique<TrackerManager>(detectedBox[umd], detected[umd].point));
//     }
// }
