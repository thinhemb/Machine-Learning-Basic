/********************************************************************************
<Copyright Notice>
Product: AI Camera
Module: AI Core
Version: 2.0
File: Litter.cpp
Author: QuangNTd
Created: 06-Sep-22
Modified:
    <Name> 
    <Date> 
    <Change>
Released: <Date>
Description: Implement of class Speed
Note: <Note>
********************************************************************************/

#include "Speed.hpp"

/*-------------------------------------------------------------------------------
Function: Speed()
Purpose: Construct a new Speed object
Parameters: 
Return: 
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
Speed::Speed()
{
    m_pDetector = new ObjectDetector();
    m_pTracker = new ObjectTracking();
}

/*-------------------------------------------------------------------------------
Function: ~Speed()
Purpose: Destroy the Speed object
Parameters: 
Return: 
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
Speed::~Speed()
{
    if (m_pDetector != nullptr) delete m_pDetector;
    if (m_pTracker != nullptr) delete m_pTracker;
}

/*-------------------------------------------------------------------------------
Function: int SP_Init()
Purpose: Initialize the Speed object
Parameters: 
Return: 
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
int Speed::SP_Init()
{
    if (m_pDetector->initObjectDetector("snpe", "traffic", "DSP") != STATUS_SUCCESS)
    {
        // std::cerr << TagLitter << " Initialize Litter object failed." << std::endl;
        return -1;
    }

    return 0;
}

/*-------------------------------------------------------------------------------
Function: int SP_Set(stSettingSpeed_t stSetting)
Purpose: Set up the Speed object
Parameters: stSetting
Return: 
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
int Speed::SP_Set(stSettingSpeed_t stSetting)
{
    m_ptRegion = stSetting.ptRegionSet;
    m_strObject = stSetting.strObjectSet;

    return 0;
}

/*-------------------------------------------------------------------------------
Function: int SP_Exe(cv::Mat& mImage, std::vector<stOutputSpeed_t>& stSpeeds)
Purpose: Execute the Speed object
Parameters: mImage, stSpeeds
Return: 
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
int Speed::SP_Exe(cv::Mat& mImage, std::vector<stOutputSpeed_t>& stSpeeds)
{
    stSpeeds.clear();
    if (mImage.empty())
    {
        std::cerr << TagSpeed << " Input invalid." << std::endl;
        return -1;
    }
    else
    {
        cv::Mat mCopyImage;
        mImage.copyTo(mCopyImage);

        // detect object
        std::vector<ObjectTrace> stObjects;
        
        if (m_pDetector->executeObjectDetector(mImage, stObjects, THRESH_SPEED) != STATUS_SUCCESS)
        {
            std::cerr << TagSpeed << " Execute Object Detector failed." << std::endl;
            return -1;
        }
        std::cout<<"======"<<stObjects.size()<<std::endl;
        // tracking 
        std::vector<TrackingTrace> stTracks;
        m_pTracker->process(stObjects, stTracks);
        // delete lose track
        for (auto it = m_stSpeedTracks.begin(); it != m_stSpeedTracks.end(); )
        {
            const int iID = (*it).iTrackID;
            const auto p = find_if(stTracks.begin(), stTracks.end(), 
                                    [iID] (const TrackingTrace& st) { return (st.m_ID == iID); });
            if (p == stTracks.end() && it != m_stSpeedTracks.end())
            {
                it = m_stSpeedTracks.erase(it);
            }
            else
            {
                it ++;
            }
        }

        for (auto& stTrack : stTracks)
        {
            if (stTrack.isOutOfFrame == false)
            {
                cv::rectangle(mCopyImage, stTrack.m_rect, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);
                std::string text{std::to_string(stTrack.m_ID) + " - " + stTrack.m_type};
			    cv::putText(mCopyImage, text, cv::Point(stTrack.m_rect.x, stTrack.m_rect.y), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 255), 1);

                int iID = stTrack.m_ID;
                const auto p = find_if(m_stSpeedTracks.begin(), m_stSpeedTracks.end(),
                                        [iID] (const stOutputSpeed_t& a) { return (a.iTrackID == iID); });
                if (p != m_stSpeedTracks.end()) // if find object in list tracked object
                {
                    int iIndex = distance(m_stSpeedTracks.begin(), p);
                    
                    m_stSpeedTracks[iIndex].riBox = stTrack.m_rect;
                    m_stSpeedTracks[iIndex].bOutOfFrame = false;
                    m_stSpeedTracks[iIndex].bEvent = false;
                }
                else // if find new object
                {
                    stOutputSpeed_t stSpeed;
                    stSpeed.iTrackID = stTrack.m_ID;
                    stSpeed.strType = stTrack.m_type;
                    stSpeed.riBox = stTrack.m_rect;
                    stSpeed.bEvent =true;
                    m_stSpeedTracks.push_back(stSpeed);
                }
            }
            else
            {
                int iID = stTrack.m_ID;
                const auto p = find_if(m_stSpeedTracks.begin(), m_stSpeedTracks.end(),
                                        [iID] (const stOutputSpeed_t& a) { return (a.iTrackID == iID); });
                if (p != m_stSpeedTracks.end())
                {
                    int iIndex = distance(m_stSpeedTracks.begin(), p);

                    m_stSpeedTracks[iIndex].bOutOfFrame = true;
                }
            }
        }

        for (auto stSpeed : m_stSpeedTracks)
        {
            if (stSpeed.bOutOfFrame != true)
            {
                if (stSpeed.bEvent == true)
                {
                    stSpeeds.push_back(stSpeed);
                }
            }
        }

        return 0;
    }
}