/********************************************************************************
<Copyright Notice>
Product: AI Camera
Module: AI Core
Version: 2.0
File: Litter.hpp
Author: QuangNTd
Created: 14-Jul-22
Modified:
    <Name>
    <Date>
    <Change>
Released: <Date>
Description: Header of class Litter
Note: <Note>
********************************************************************************/

#ifndef Litter_hpp
#define Litter_hpp

#define TagLitter "[Litter]"

#include "ObjectDetector.hpp"
#include "ObjectTracking.hpp"

#define THRESH_TRASH 0.4
#define THRESH_OBJECT 0.4

/**
 * @brief Structure of configuration Litter
 * 
 */
typedef struct stSettingLitter
{
    std::vector<cv::Point> ptRegionSet;
    std::vector<std::string> strObjectSet;
} stSettingLitter_t;

/**
 * @brief Enumeration of trash state
 * 
 */
typedef enum emTrashState
{
    Hold = 0,
    ThrowedIn = 1,
    ThrowedOut = 2,
    Undefined = -1
} emTrashState_t;

/**
 * @brief Structure of trash object
 * 
 */
typedef struct stTrash
{
    cv::Rect riBox;
    std::string strType = "";
    int iTrackID = 0;
    bool bOutOfFrame = false;
    emTrashState_t emState = emTrashState_t::Undefined;
    int iOwnID = -1;
} stTrash_t;

/**
 * @brief Structure of output Litter
 * 
 */
typedef struct stOutputLitter
{
    cv::Rect riBox;
    std::string strType = "";
    int iTrackID = 0;
    bool bOutOfFrame = false;
    bool bInside = false;
    stTrash_t stTrash;
    bool bEvent = false;
} stOutputLitter_t;

/**
 * @brief Class that contains the functions of Litter
 * 
 */
class Litter
{
    public:
        /**
         * @brief Construct a new Litter object
         * 
         */
        Litter();

        /**
         * @brief Destroy the Litter object
         * 
         */
        ~Litter();

        /**
         * @brief Initialize the Litter object
         * 
         * @return int : 0 if initialized successfully; -1, otherwise
         */
        int LT_Init();

        /**
         * @brief Set up the Litter object
         * 
         * @param stSetting Structure of configuration Litter
         * @return int : 0 if initialized successfully; -1, otherwise
         */
        int LT_Set(stSettingLitter_t stSetting);

        /**
         * @brief Execute the Litter object
         * 
         * @param mImage Input image format cv::Mat
         * @param[in,out] stLitters A vector of stOutputLitter_t that will hold the output data
         * @return int : 0 if initialized successfully; -1, otherwise
         */
        int LT_Exe(cv::Mat& mImage, std::vector<stOutputLitter_t>& stLitters);

    private:
        ObjectDetector* m_pObjectDetector = nullptr;
        ObjectDetector* m_pTrashDetector = nullptr;
        ObjectTracking* m_pObjectTracker = nullptr;
        ObjectTracking* m_pTrashTracker = nullptr;

        std::vector<cv::Point> m_ptRegion;
        std::vector<std::string> m_strObject;

        std::vector<stTrash_t> m_stTrashTracks;
        std::vector<stOutputLitter_t> m_stObjectTracks;

    private:
        /**
         * @brief 
         * 
         * @param stTrack 
         * @param ptArea 
         * @return true 
         * @return false 
         */
        bool LT_bCheckInside(TrackingTrace& stTrack, std::vector<cv::Point> ptArea);

        /**
         * @brief 
         * 
         * @param stTrashTrack 
         * @param stLitter 
         * @return true 
         * @return false 
         */
        bool LT_bCheckState(TrackingTrace& stTrashTrack, stOutputLitter_t& stLitter);
};

#endif // Litter_hpp