/********************************************************************************
<Copyright Notice>
Product: AI Camera
Module: AI Core
Version: 2.0
File: ITScommon.hpp
Author: QuangNTd
Created: 11-Jul-22
Modified:
    <Name>
    <Date>
    <Change>
Released: <Date>
Description: Header of common functions for ITS
Note: <Note>
********************************************************************************/

#ifndef ITScommon_hpp
#define ITScommon_hpp

#include "regex"

// #include "AiTypeData.hpp"
#include "ocr_db_crnn.hpp"
#include "ObjectTracking.hpp"

#define THRESHOLD_OCR 0.9

#define MAX_COUNT_UNKNOWN 5

#define WIDTH_4K 3840
#define HEIGHT_4K 2160
#define SCALE_FHD_4K 2

/**
 * Enumeration of 
 */
typedef enum emNation
{
    VietNam = 0,
    USA = 1,
    Malaysia = 2
} emNation_t;

/**
 * Structure of rectText.
 */
typedef struct stRectText
{
    std::vector<cv::Point> points;
    cv::Point minPoint;
} stRectText_t;

/**
 * Structure of configuration for ocr module.
 */
typedef struct stOcrConfig
{
    std::vector<std::string> dictionary;
    std::map<std::string, double> configOCR;
    std::shared_ptr<PaddlePredictor> detector;
    std::shared_ptr<PaddlePredictor> recognizer;
} stOcrConfig_t;

/**
 * Structure of information license plate.
 */
typedef struct stPlateInfor
{
    cv::Mat mCropPlate;
    cv::Rect riBoxPlate;
    std::string strLicense = "";
    std::string strTypePlate = "";
    bool bDefectPlate = false;
    float fScore;
    int iTrackID = 0;
    bool bOutOfFrame = false;
    bool bEvent = false;
    uint8_t uiCountUnknown = 0;
} stPlateInfor_t;

/**
 * Structure of parameters setting for Vehicle.
 */
typedef struct stSettingVehicle
{
    std::vector<std::string> strVehicles;
    emNation_t emTypeNation;
} stSettingVehicle_t;

/**
 * Structure of information vehicle.
 */
typedef struct stVehicleInfor
{
    cv::Mat mCropVehicle;
    cv::Rect riBoxVehicle;
    std::string strTypeVehicle = "";
    float fScore;
    int iTrackID = 0;
    double dDistance = 0;
    bool bOutOfFrame = false;
    uint8_t uiCountGoOut = 0;
    bool bHavePlate = false;
    bool bEvent = false;
    stPlateInfor_t stPlateInfor;
    bool bObjectTrack = false;
    // Attributes attributes;
} stVehicleInfor_t;

/**
 * @brief 
 * 
 * @param emTypeNation 
 * @return stOcrConfig* 
 */
stOcrConfig* ITS_InitOcr(emNation_t emTypeNation);

/**
 * @brief 
 * 
 * @param pTracker 
 * @param emTypeNation 
 * @param stObjects 
 * @param stTracks 
 * @return int 
 */
int ITS_PlateTracking(ObjectTracking* & pTracker, emNation_t emTypeNation, std::vector<ObjectTrace> stObjects, std::vector<TrackingTrace>& stTracks);

/**
 * @brief 
 * 
 * @param stPlate 
 * @param iWidthImage 
 * @param iHeightImage 
 * @return true 
 * @return false 
 */
bool ITS_CheckPlate(stPlateInfor_t& stPlate, int iWidthImage, int iHeightImage);

/**
 * @brief 
 * 
 * @param rfBoxFhd 
 * @param iCols4k 
 * @param iRows4k 
 * @return cv::Rect2f 
 */
cv::Rect2f ITS_GetBox4k(cv::Rect2f& rfBoxFhd, int iCols4k, int iRows4k);

/**
 * @brief 
 * 
 * @param boxes 
 * @return std::vector<std::vector<std::vector<int>>> 
 */
std::vector<std::vector<std::vector<int>>> ITS_SortBoxes(std::vector<std::vector<std::vector<int>>> boxes);

/**
 * @brief 
 * 
 * @param strLicense 
 * @param emTypeNation 
 * @return std::string 
 */
std::string ITS_CheckLicense(std::string& strLicense, emNation_t emTypeNation);

/**
 * @brief 
 * 
 * @param stOcrConfig 
 * @param mCropPlate4k 
 * @param emTypeNation 
 * @param strTypePlate 
 * @param fConfidence 
 * @return std::string 
 */
std::string ITS_ReadText(stOcrConfig_t* & stOcrConfig, cv::Mat& mCropPlate4k, emNation_t emTypeNation, std::string strTypePlate, float& fConfidence);

// /**
//  * @brief 
//  * 
//  * @tparam _stTracks 
//  * @param stTracks 
//  * @param stObjectTracks 
//  */
// template <typename _stTracks>
// void ITS_DeleteLoseTrack(_stTracks& stTracks, std::vector<TrackingTrace>& stObjectTracks);

#endif // ITScommon_hpp