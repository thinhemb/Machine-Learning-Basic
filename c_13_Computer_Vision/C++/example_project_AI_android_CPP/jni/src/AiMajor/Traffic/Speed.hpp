/********************************************************************************
<Copyright Notice>
Product: AI Camera
Module: AI Core
Version: 2.0
File: Litter.hpp
Author: QuangNTd
Created: 06-Sep-22
Modified:
    <Name> 
    <Date> 
    <Change> 
Released: <Date>
Description: Header of class Speed
Note: <Note>
********************************************************************************/

#ifndef Speed_hpp
#define Speed_hpp

#define TagSpeed "[Speed]"

#include "ObjectDetector.hpp"
#include "ObjectTracking.hpp"

#define THRESH_SPEED 0.4

/**
 * @brief Structure of configuration Speed
 * 
 */
typedef struct stSettingSpeed
{
    std::vector<cv::Point> ptRegionSet;
    std::vector<std::string> strObjectSet;
} stSettingSpeed_t;

/**
 * @brief Structure of output Speed
 * 
 */
typedef struct stOutputSpeed
{
    cv::Rect riBox;
    std::string strType = "";
    int iTrackID = 0;
    bool bOutOfFrame = false;
    int iSpeed = 0;
    bool bEvent = false;
} stOutputSpeed_t;

/**
 * @brief Class that contains the functions of Speed
 * 
 */
class Speed
{
    public:
        /**
         * @brief Construct a new Speed object
         * 
         */
        Speed();

        /**
         * @brief Destroy the Speed object
         * 
         */
        ~Speed();

        /**
         * @brief Initialize the Speed object
         * 
         * @return int : 0 if initialized successfully; -1, otherwise
         */
        int SP_Init();

        /**
         * @brief Set up the Speed object
         * 
         * @param stSetting Structure of configuration Speed
         * @return int : 0 if initialized successfully; -1, otherwise
         */
        int SP_Set(stSettingSpeed_t stSetting);

        /**
         * @brief Execute the Speed object
         * 
         * @param mImage Input image format cv::Mat
         * @param[in,out] stSpeeds A vector of stOutputSpeed_t that will hold the output data
         * @return int : 0 if initialized successfully; -1, otherwise
         */
        int SP_Exe(cv::Mat& mImage, std::vector<stOutputSpeed_t>& stSpeeds);

    private:
        ObjectDetector* m_pDetector = nullptr;
        ObjectTracking* m_pTracker = nullptr;

        std::vector<cv::Point> m_ptRegion;
        std::vector<std::string> m_strObject;

        std::vector<stOutputSpeed_t> m_stSpeedTracks;

};

#endif // Speed_hpp