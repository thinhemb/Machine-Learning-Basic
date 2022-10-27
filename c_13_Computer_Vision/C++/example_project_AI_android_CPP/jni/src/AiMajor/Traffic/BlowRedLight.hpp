/********************************************************************************
<Copyright Notice>
Product: AI Camera
Module: AI Core
Version: 2.0
File: BlowRedLight.hpp
Author: QuangNTd
Created: 12-Jul-22
Modified:
    <Name> QuangNTd
    <Date> 17-Aug-22
    <Change>
Released: <Date>
Description: Header of class BlowRedLight
Note: <Note>
********************************************************************************/

#ifndef BlowRedLight_hpp
#define BlowRedLight_hpp

#define TagBRL "[BlowRedLight]"

#include "ObjectDetector.hpp"
#include "ObjectTracking.hpp"

#define THRESH_BRL 0.4

/**
 * @brief Structure of configuration BlowRedLight
 * 
 */
typedef struct stSettingBlowRedLight
{
    std::vector<std::string> strAllowedObjects;
    std::vector<std::vector<cv::Point>> ptRegionsSet;
    std::vector<std::string> strLabelsSet;
    int iAllowedLeft = 0;
    int iAllowedRight = 0;
    int iWidthFrame = 0;
    int iHeightFrame = 0;
} stSettingBlowRedLight_t;

/**
 * @brief Enumeration of traffic light state
 * 
 */
typedef enum emLightState
{
    Off = 0,
    Red = 1,
    Yellow = 2,
    Green = 3,
    Undefined = -1
} emLightState_t;

/**
 * @brief Structure of output BlowRedLight
 * 
 */
typedef struct stOutputBlowRedLight
{
    cv::Rect riBox;
    std::string strType = "";
    int iTrackID = 0;
    bool bOutOfFrame = false;
    bool bEvent = false;
    bool bLeft = false;
    bool bRight = false;
    bool bStraight = false;
    cv::Mat mImageEvent;
} stOutputBlowRedLight_t;

/**
 * @brief Class that contains the functions of BlowRedLight
 * 
 */
class BlowRedLight
{
    public:
        /**
         * @brief Construct a new Blow Red Light object
         * 
         */
        BlowRedLight();

        /**
         * @brief Destroy the Blow Red Light object
         * 
         */
        ~BlowRedLight();

        /**
         * @brief Initialize the BlowRedLight object
         * 
         * @return int : 0 if initialized successfully; -1, otherwise
         */
        int BRL_Init();

        /**
         * @brief Set up the BlowRedLight object
         * 
         * @param stSetting Structure of configuration BlowRedLight
         * @return int : 0 if initialized successfully; -1, otherwise
         */
        int BRL_Set(stSettingBlowRedLight_t stSetting);

        /**
         * @brief Execute the BlowRedLight object
         * 
         * @param mImage Input image format cv::Mat
         * @param[in,out] stBlowRedLights A vector of stOutputBlowRedLight_t that will hold the output data
         * @return int : 0 if initialized successfully; -1, otherwise
         */
        int BRL_Exe(cv::Mat& mImage, std::vector<stOutputBlowRedLight_t>& stBlowRedLights);

    private:
        ObjectDetector* m_pDetector = nullptr;
        ObjectTracking* m_pTracker = nullptr;

        std::vector<std::string> m_strAllowedObjects;
        std::vector<std::vector<cv::Point>> m_ptStandardLight, m_ptLeftLight;
        std::vector<cv::Point> m_ptBeginLine, m_ptEndLine, m_ptLeftLine, m_ptRightLine;
        std::vector<cv::Point> m_ptFullLane, m_ptStraightLane, m_ptLeftLane, m_ptRightLane, m_ptStraightLeftLane, m_ptStraightRightLane;
        emLightState_t m_emLeftSignal = emLightState_t::Undefined;
        emLightState_t m_emStandardSignal = emLightState_t::Undefined;
        int m_iAllowedLeft, m_iAllowedRight;

        int m_iWidthFrame, m_iHeightFrame;
        cv::Point2f m_ptBeginLineB, m_ptBeginLineE;
        cv::Point2f m_ptEndLineB, m_ptEndLineE;
        cv::Point2f m_ptLeftLineB, m_ptLeftLineE;
        cv::Point2f m_ptRightLineB, m_ptRightLineE;

        // bool m_bCheckBegin = false;
        // bool m_bCheckEnd = false;
        // bool m_bCheckLeft = false;
        // bool m_bCheckRight = false;

        std::vector<stOutputBlowRedLight_t> m_stTracks;

    private:
        /**
         * @brief 
         * 
         * @param mImage 
         * @param ptLights 
         * @return emLightState_t 
         */
        emLightState_t BRL_emGetLightState(cv::Mat& mImage, std::vector<std::vector<cv::Point>> ptLights);

        /**
         * @brief 
         * 
         * @param iAllowedTurn 
         * @param strType 
         * @return true 
         * @return false 
         */
        bool BRL_bCheckAllowedTurn(int iAllowedTurn, std::string strType);
};

#endif // BlowRedLight_hpp