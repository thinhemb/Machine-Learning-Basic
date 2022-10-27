#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <iostream>
#include <vector>

#include "opencv2/opencv.hpp"

#define PATH_DICTIONARY "/system/app/bkavai/models/paddle/anpr.txt"
#define PATH_DICTIONARY_CHINA "/system/app/bkavai/models/paddle/china-dic-convert.txt"

#define PATH_OCR_DET_VN "/system/app/bkavai/models/paddle/ocr_detect_vn.nb" 
#define PATH_OCR_RECOG_VN "/system/app/bkavai/models/paddle/ocr_recog_vn.nb"

#define PATH_OCR_DET_US "/system/app/bkavai/models/paddle/us-det.nb" 
#define PATH_OCR_RECOG_US "/system/app/bkavai/models/paddle/us-rec.nb"

#define PATH_OCR_DET_MALAY "/system/app/bkavai/models/paddle/malay-det.nb"
#define PATH_OCR_RECOG_MALAY "/system/app/bkavai/models/paddle/malay-rec.nb"

#define PATH_OCR_DET_CHINA "/system/app/bkavai/models/paddle/china-det.nb"
#define PATH_OCR_RECOG_CHINA "/system/app/bkavai/models/paddle/china-rec.nb"

#define DIR_MODEL_OBJECTDETECION_SNPE "/system/app/bkavai/models/snpe/"
#define DIR_LABEL_OBJECTDETECION_SNPE "/system/app/bkavai/models/snpe/"

struct ObjectTrace
{
    cv::Rect2f rect;
    std::string label;
    float score;
    int obj_id;
    cv::Rect2f rect_scale;
};

#define STATUS_FAILED -1
#define STATUS_SUCCESS 0

#define LOG_INFO_(log) std::cout << "[INFO] - " << log << std::endl;
#define LOG_SUCCESS(log) std::cout << "[SUCCESS] - " << log << std::endl;
#define LOG_FAIL(log) std::cout << "[FAILED] - " << log << std::endl;

#endif
