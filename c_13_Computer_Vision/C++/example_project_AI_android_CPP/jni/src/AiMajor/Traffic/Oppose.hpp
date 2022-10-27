/********************************************************************************
<Copyright Notice>
Product: AI Camera
Module: AI Core
Version: 2.0
File: Oppose.hpp
Author: QuangNTd
Created: 12-Jul-22
Modified:
    <Name>
    <Date>
    <Change>
Released: <Date>
Description: Header of class Oppose
Note: <Note>
********************************************************************************/

#ifndef Oppose_hpp
#define Oppose_hpp

#define TagOppose "[Oppose]"

#include "ObjectDetector.hpp"
#include "ObjectTracking.hpp"

#define THRESH_OPPOSE 0.4
#define MAX_COUNT_CHECK 3
#define THRESH_TRACE 25

/**
 * @brief Structure of direction configuration
 * 
 */
typedef struct stDirection
{
    cv::Point ptHead;
    cv::Point ptTail;
} stDirection_t;

/**
 * @brief Structure of configuration Oppose
 * 
 */
typedef struct stSettingOppose
{
    std::vector<cv::Point> ptRegionSet;
    stDirection_t stDirectionSet;
    std::vector<std::string> strVehicleSet;
} stSettingOppose_t;

/**
 * @brief Structure of output Oppose
 * 
 */
typedef struct stOutputOppose
{
    cv::Rect riBox;
    std::string strType = "";
    int iTrackID = 0;
    bool bOutOfFrame = false;
    bool bEvent = false;
    int iCountCheck = 0;
} stOutputOppose_t;

/**
 * @brief Class that contains the functions of Oppose
 * 
 */
class Oppose
{
    public:
        /**
         * @brief Construct a new Oppose object
         * 
         */
        Oppose();

        /**
         * @brief Destroy the Oppose object
         * 
         */
        ~Oppose();

        /**
         * @brief Initialize the Oppose object
         * 
         * @return int : 0 if initialized successfully; -1, otherwise
         */
        int OP_Init();

        /**
         * @brief Set up the Oppose object
         * 
         * @param stSetting Structure of configuration Oppose
         * @return int : 0 if initialized successfully; -1, otherwise
         */
        int OP_Set(stSettingOppose_t stSetting);

        /**
         * @brief Execute the Oppose object
         * 
         * @param mImage Input image format cv::Mat
         * @param[in,out] stOpposes A vector of stOutputOppose_t that will hold the output data
         * @return int : 0 if initialized successfully; -1, otherwise
         */
        int OP_Exe(cv::Mat& mImage, std::vector<stOutputOppose_t>& stOpposes);

    private:
        ObjectDetector* m_pDetector = nullptr;
        ObjectTracking* m_pTracker = nullptr;

        std::vector<cv::Point> m_ptRegion;
        stDirection_t m_stDirection;
        std::vector<std::string> m_strVehicle;

        bool bLess;

        std::vector<stOutputOppose_t> m_stTracks;
    
    private:
        /**
         * @brief 
         * 
         * @param stTrack 
         * @param ptArea 
         * @return true 
         * @return false 
         */
        bool OP_bCheckLane(TrackingTrace stTrack, std::vector<cv::Point> ptArea);

        /**
         * @brief 
         * 
         * @param stTrack 
         * @return float 
         */
        float OP_fCalDistanceTrace(TrackingTrace stTrack);
};

#endif // Oppose_hpp