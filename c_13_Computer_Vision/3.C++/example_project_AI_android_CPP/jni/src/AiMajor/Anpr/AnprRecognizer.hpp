#ifndef AnprRecognizer_hpp
#define AnprRecognizer_hpp

#include <regex>
#include <pthread.h>

#include "ocr_db_crnn.hpp"
#include "AnprDetector.hpp"
#include "ObjectTracking.hpp"
// #include "MotionDetector.hpp"

#define MAX_COUNT_UNKNOWN 5
#define MAX_COUNT_VERIFY 3
#define THRESHOLD_ANPR 0.8
#define THRESHOLD_OCR 0.9

#define WIDTH_4K 3840
#define HEIGHT_4K 2160
#define SCALE_FHD_4K 2

using namespace paddle::lite_api; // NOLINT
using namespace std;
using namespace cv;

typedef struct rectText
{
    std::vector<cv::Point> points;
    cv::Point minPoint;
} rectText; 

typedef struct OcrConfig
{
    std::vector<std::string> dict;
    std::map<std::string, double> configOCR;
    std::shared_ptr<PaddlePredictor> detector;
    std::shared_ptr<PaddlePredictor> recog;
} OcrConfig;

struct PlateInfor
{
    cv::Mat imgPlate;
    cv::Rect rect;
    std::string license = "";
    std::string typePlate = "";
    bool isDefectPlate = false;
    float score;
    int track_id = 0;
    bool isOutOfFrame = false;
    bool isNewEvent = false;
    uint8_t countUnknown = 0;
    // uint8_t countVerify = 0; // trackAnprVerify
    // std::vector<std::string> licenseVec; // trackAnprVerify
};

struct settingVehicle
{
    std::vector<std::string> objectsTrack;
    Nations nation;
};

// std::vector<std::string> listGetAttributes = {"car", "van", "ambulance", "truck", "firetruck", "tanktruck", 
//                                                 "trashcar", "crane", "container", "bus"};

struct VehicleInfor
{
    cv::Mat imgVehicle;
    cv::Rect rectVehicle;
    std::string typeVehicle = "";
    int iTrackID = 0;
    double dDistance = 0;
    bool bOutOfFrame = false;
    uint8_t uiCountGoOut = 0;
    bool bHavePlate = false;
    bool bEvent = false;
    PlateInfor plateInf;
    bool onObjectsTrack = false;
    // Attributes attributes;
};

class AnprRecognizer
{ 
private:
    /* data */
    Nations nations;
    // MotionDetector motion;
    AnprDetector* detector = nullptr;
    ObjectTracking* tracker = nullptr;
    ObjectDetector* vehicle_detector = nullptr;
    ObjectTracking* m_pVehicleTracker = nullptr;
    // ObjectDetector* vehicle_attributes = nullptr;

    OcrConfig* ocrVN = nullptr;
    OcrConfig* ocrUS = nullptr;
    OcrConfig* ocrMalay = nullptr;
    OcrConfig* ocrChina = nullptr;

    std::vector<PlateInfor> listEvent; // recognize

    std::vector<std::string> objectsTrack;
    std::vector<PlateInfor> listPlateTracks;
    std::vector<VehicleInfor> listVehicleTracks;

    std::string readText( cv::Mat& img, Nations nation, float& confidence);
    std::vector<std::vector<std::vector<int>>> sortBoxes(std::vector<std::vector<std::vector<int>>> boxes);
    int initVn(std::string pathDet, std::string pathRecog);
    int initVnSquare(std::string pathDet, std::string pathRecog);
    int initVnRect(std::string pathDet, std::string pathRecog);
    int initUS(std::string pathDet, std::string pathRecog);
    int initMalay(std::string pathDet, std::string pathRecog);
    bool isValidPlate(cv::Mat& img);
    bool checkPlate(PlateInfor& plate, int cols, int rows);
    void getAttribute(VehicleInfor& vehicle, std::vector<ObjectTrace> listVehicle);
    // void getAttributes(cv::Mat& img, VehicleInfor& vehicle, std::vector<ObjectTrace> listVehicle);
    std::string checkLicenseVN(std::string& text);
    cv::Rect2f getRect4K(cv::Rect2f rect_fhd, int cols_4K, int rows_4K);

    // vehicle
    bool CheckOutside(TrackingTrace& stTrack, int iWidthImage, int iHeightImage);
    void MappingVehicle(PlateInfor& stPlate, std::vector<VehicleInfor>& stVehicleTracks);
    void UpdateVehicle(PlateInfor& stPlate, std::vector<VehicleInfor>& stVehicleTracks);

    //
    int initChina(std::string pathDet, std::string pathRecog);

public:
    AnprRecognizer(/* args */);
    ~AnprRecognizer();
    int init(Nations nation);
    int initVehicle(settingVehicle settings);
    int recognize(cv::Mat& img, cv::Mat &img_4K, std::vector<PlateInfor>& plates); // 4K
    // int recognize(cv::Mat& img, std::vector<PlateInfor>& plates, bool checkIsMotion, Rect& rectMotion);
    int trackAnpr( Mat &img, cv::Mat &img_4K, std::vector<PlateInfor>& plates); // 4K
    int trackAnprVerify(Mat &img, std::vector<PlateInfor>& plates);
    int trackVehicle( Mat &img, cv::Mat &img_4K, std::vector<VehicleInfor>& vehicles); // 4K

    int trackAnpr_cn( Mat &img, cv::Mat &img_4K, std::vector<PlateInfor>& plates); // 4K

    int initVehicle_lt(settingVehicle stSetVehicle);
    int trackVehicle_lt(cv::Mat& mImage, cv::Mat& mImage4K, std::vector<VehicleInfor>& stVehicles); // lt

    int threadVehicle(cv::Mat &img, cv::Mat &img_4K, std::vector<VehicleInfor>& vehicles); // 4K
public:
    std::vector<ObjectTrace> objVehicles;
    std::vector<TrackingTrace> tracks;
    friend void *thread_VehicleDetect(void *args);
    friend void *thread_PlateTracking(void *args);
};

struct arg_VehicleDetect
{
    AnprRecognizer *p;
    
    cv::Mat img;
    std::vector<ObjectTrace> objVehicles;
};

struct arg_PlateTracking
{
    AnprRecognizer *p;

    cv::Mat img;
    std::vector<TrackingTrace> tracks;
};

#endif