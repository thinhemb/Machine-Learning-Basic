#include "Util.hpp"

float iou(cv::Rect2f& rect1, cv::Rect2f& rect2) 
{
    int x1 = std::max(rect1.x, rect2.x);
    int y1 = std::max(rect1.y, rect2.y);
    int x2 = std::min(rect1.x + rect1.width, rect2.x + rect2.width);
    int y2 = std::min(rect1.y + rect1.height, rect2.y + rect2.height);
    int w = std::max(0, (x2 - x1 + 1));
    int h = std::max(0, (y2 - y1 + 1));
    float inter = w * h;
    float area1 = rect1.width * rect1.height;
    float area2 = rect2.width * rect2.height;
    float o = inter / (area1 + area2 - inter);
    return (o >= 0) ? o : 0;
}

void nms(std::vector<ObjectTrace>& boxes, const double nms_threshold)
{
    std::vector<int> scores;
    for(int i=0; i < boxes.size(); i++) {
        scores.push_back(boxes[i].score);
    }
    std::vector<int> index;
    for(int i=0; i < scores.size(); i++) {
        index.push_back(i);
    }

    std::sort(index.begin(), index.end(), [&](int a, int b){return scores[a] > scores[b];});
    
    std::vector<bool> del(scores.size(), false);
    for(size_t i=0; i < index.size(); i++) {
        if(!del[index[i]]) {
            for(size_t j=i+1; j < index.size(); j++) {
                if(iou(boxes[index[i]].rect, boxes[index[j]].rect) > nms_threshold) {
                    del[index[j]] = true;
                }
            }
        }
    }
    std::vector<ObjectTrace> new_obj;
    for(const auto i : index) {
        ObjectTrace obj;
        if(!del[i]) {
            obj.obj_id = boxes[i].obj_id;
            obj.label = boxes[i].label;
            obj.score = boxes[i].score;
            obj.rect.x = boxes[i].rect.x;
            obj.rect.y = boxes[i].rect.y;
            obj.rect.width = boxes[i].rect.width;
            obj.rect.height = boxes[i].rect.height;
        }
        new_obj.push_back(obj);
    }
    boxes = new_obj;
}