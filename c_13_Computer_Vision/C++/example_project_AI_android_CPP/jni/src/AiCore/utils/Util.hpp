#ifndef Util_hpp
#define Util_hpp

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "AiTypeData.hpp"

float iou(cv::Rect2f& rect1, cv::Rect2f& rect2);
void nms(std::vector<ObjectTrace>& boxes, const double nms_threshold);

#endif // Util_hpp