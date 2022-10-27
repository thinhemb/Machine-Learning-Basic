/********************************************************************************
<Copyright Notice>
Product: AI Camera
Module: AI Core
Version: 2.0
File: AnprRecognizer.cpp
Author: QuangNTd
Created: 12-Jul-22
Modified:
    <Name>
    <Date>
    <Change>
Released: <Date>
Description: Implement of class BlowRedLight
Note: <Note>
********************************************************************************/

#include "BlowRedLight.hpp"

/*-------------------------------------------------------------------------------
Function: BlowRedLight()
Purpose: Construct a new BlowRedLight object
Parameters: 
Return: 
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
BlowRedLight::BlowRedLight()
{
    m_pDetector = new ObjectDetector();
    m_pTracker = new ObjectTracking();
}

/*-------------------------------------------------------------------------------
Function: ~BlowRedLight()
Purpose: Destroy the Blow Red Light object
Parameters: 
Return: 
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
BlowRedLight::~BlowRedLight()
{    
    if (m_pDetector != nullptr)
    {
        delete m_pDetector;
    }
    
    if (m_pTracker != nullptr)
    {
        delete m_pTracker;
    }
}

/*-------------------------------------------------------------------------------
Function: int BRL_Init()
Purpose: Initialize the BlowRedLight object
Parameters: 
Return: 
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
int BlowRedLight::BRL_Init()
{
    if (m_pDetector->initObjectDetector("snpe", "traffic", "DSP") != STATUS_SUCCESS)
    {
        // std::cerr << TagBRL << " Initialize BlowRedLight object failed." << std::endl;
        return -1;
    }

    return 0;
}

/*-------------------------------------------------------------------------------
Function: int BRL_Set(stSettingBlowRedLight_t stSetting)
Purpose: Set up the BlowRedLight object
Parameters: stSetting
Return: 
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
int BlowRedLight::BRL_Set(stSettingBlowRedLight_t stSetting)
{
    m_strAllowedObjects = stSetting.strAllowedObjects;
    m_iAllowedLeft = stSetting.iAllowedLeft;
    m_iAllowedRight = stSetting.iAllowedRight;
    m_iWidthFrame = stSetting.iWidthFrame;
    m_iHeightFrame = stSetting.iHeightFrame;

    if ((stSetting.ptRegionsSet.size() != 0) && (stSetting.strLabelsSet.size() != 0)
        && (stSetting.ptRegionsSet.size() == stSetting.strLabelsSet.size()))
    {
        for (int i = 0; i < stSetting.strLabelsSet.size(); i++)
        {
            if (stSetting.strLabelsSet[i] == "0")
            {
                m_ptLeftLight.push_back(stSetting.ptRegionsSet[i]);
            }
            else if (stSetting.strLabelsSet[i] == "1")
            {
                m_ptStandardLight.push_back(stSetting.ptRegionsSet[i]);
            }
            else if (stSetting.strLabelsSet[i] == "2")
            {
                m_ptBeginLine = stSetting.ptRegionsSet[i];
                m_ptBeginLineB.x = float(m_ptBeginLine[0].x) / float(m_iWidthFrame);
                m_ptBeginLineB.y = float(m_ptBeginLine[0].y) / float(m_iHeightFrame);
                m_ptBeginLineE.x = float(m_ptBeginLine[1].x) / float(m_iWidthFrame);
                m_ptBeginLineE.y = float(m_ptBeginLine[1].x) / float(m_iHeightFrame);
                m_bCheckBegin = true;
            }
            else if (stSetting.strLabelsSet[i] == "3")
            {
                m_ptEndLine = stSetting.ptRegionsSet[i];
                m_ptEndLineB.x = float(m_ptEndLine[0].x) / float(m_iWidthFrame);
                m_ptEndLineB.y = float(m_ptEndLine[0].y) / float(m_iHeightFrame);
                m_ptEndLineE.x = float(m_ptEndLine[1].x) / float(m_iWidthFrame);
                m_ptEndLineE.y = float(m_ptEndLine[1].x) / float(m_iHeightFrame);
                m_bCheckEnd = true;
            }
            else if (stSetting.strLabelsSet[i] == "4")
            {
                m_ptLeftLine = stSetting.ptRegionsSet[i];
                m_ptLeftLineB.x = float(m_ptLeftLine[0].x) / float(m_iWidthFrame);
                m_ptLeftLineB.y = float(m_ptLeftLine[0].y) / float(m_iHeightFrame);
                m_ptLeftLineE.x = float(m_ptLeftLine[1].x) / float(m_iWidthFrame);
                m_ptLeftLineE.y = float(m_ptLeftLine[1].x) / float(m_iHeightFrame);
                m_bCheckLeft = true;
            }
            else if (stSetting.strLabelsSet[i] == "5")
            {
                m_ptRightLine = stSetting.ptRegionsSet[i];
                m_ptRightLineB.x = float(m_ptRightLine[0].x) / float(m_iWidthFrame);
                m_ptRightLineB.y = float(m_ptRightLine[0].y) / float(m_iHeightFrame);
                m_ptRightLineE.x = float(m_ptRightLine[1].x) / float(m_iWidthFrame);
                m_ptRightLineE.y = float(m_ptRightLine[1].x) / float(m_iHeightFrame);
                m_bCheckRight = true;
            }
            else if (stSetting.strLabelsSet[i] == "6")
            {
                m_ptFullLane = stSetting.ptRegionsSet[i];
            }
            else if (stSetting.strLabelsSet[i] == "7")
            {
                m_ptStraightLane = stSetting.ptRegionsSet[i];
            }
            else if (stSetting.strLabelsSet[i] == "8")
            {
                m_ptStraightLeftLane = stSetting.ptRegionsSet[i];
            }
            else if (stSetting.strLabelsSet[i] == "9")
            {
                m_ptStraightRightLane = stSetting.ptRegionsSet[i];
            }
            else if (stSetting.strLabelsSet[i] == "10")
            {
                m_ptLeftLane = stSetting.ptRegionsSet[i];
            }
            else if (stSetting.strLabelsSet[i] == "11")
            {
                m_ptRightLane = stSetting.ptRegionsSet[i];
            }
        }
    }
    else
    {
        // std::cerr << TagBRL << " Set up the BlowRedLight object failed." << std::endl;
        return -1;
    }

    return 0;
}

/*-------------------------------------------------------------------------------
Function: int BRL_Exe(cv::Mat& mImage, std::vector<stOutputBlowRedLight_t>& stBlowRedLights)
Purpose: Execute the BlowRedLight object
Parameters: mImage, stBlowRedLights
Return: 
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
int BlowRedLight::BRL_Exe(cv::Mat& mImage, std::vector<stOutputBlowRedLight_t>& stBlowRedLights)
{
    stBlowRedLights.clear();
    if (mImage.empty())
    {
        // std::cerr << TagBRL << " Input invalid." << std::endl;
        return -1;
    }
    else
    {
        // cv::Mat mImageCopy;
        // mImage.copyTo(mImageCopy);

        switch (BRL_GetLightState(mImage, m_ptStandardLight))
        {
            case emLightState_t::Red:
            {
                m_emStandardSignal = emLightState_t::Red;
                break;
            }
            case emLightState_t::Yellow:
            {
                m_emStandardSignal = emLightState_t::Yellow;
                break;
            }
            case emLightState_t::Green:
            {
                m_emStandardSignal = emLightState_t::Green;
                break;
            }
            default:
                break;
        }

        // detect
        std::vector<ObjectTrace> stObjects;
        if (m_pDetector->executeObjectDetector(mImage, stObjects, THRESH_BRL) != STATUS_SUCCESS)
        {
            // std::cerr << TagBRL << " Execute Detector failed." << std::endl;
            return -1;
        }

        // tracking
        std::vector<TrackingTrace> stTracks;
        m_pTracker->process(stObjects, stTracks);
        
        // delete lose track
        for (auto it = m_stTracks.begin(); it != m_stTracks.end(); )
        {
            const int iID = (*it).iTrackID;
            const auto p = find_if(stTracks.begin(), stTracks.end(), 
                                    [iID] (const TrackingTrace& st) { return (st.m_ID == iID); });
            if (p == stTracks.end() && it != m_stTracks.end())
            {
                it = m_stTracks.erase(it);
            }
            else
            {
                it ++;
            }
        }

        // update track
        for (auto& stTrack : stTracks)
        {
            if (stTrack.isOutOfFrame == false)
            {
                int iID = stTrack.m_ID;
                const auto p = find_if(m_stTracks.begin(), m_stTracks.end(), 
                                        [iID] (const stOutputBlowRedLight_t& st) { return (st.iTrackID == iID); });
                if (p != m_stTracks.end()) // if find track in list tracked
                {
                    int iIndex = distance(m_stTracks.begin(), p);
                    m_stTracks[iIndex].riBox = stTrack.m_rect;
                    m_stTracks[iIndex].bOutOfFrame = false;

                    if (m_emStandardSignal == emLightState_t::Red)
                    {
                        cv::Mat mImageEvent = mImage.clone();
                        cv::rectangle(mImageEvent, stTrack.m_rect, cv::Scalar(0, 0, 255), 2);
                        m_stTracks[iIndex].mImageEvent = mImageEvent;
                    }

                    if ((m_iAllowedLeft == 0) && (m_iAllowedRight == 0) && (m_stTracks[iIndex].bEvent == false))
                    {
                        stBlowRedLights.push_back(m_stTracks[iIndex]);
                        m_stTracks[iIndex].bEvent = true;
                        if (m_pTracker->checkIntersection(stTrack, static_cast<float>(m_iWidthFrame), static_cast<float>(m_iHeightFrame), 
                                                            RoadLine(m_ptEndLineB, m_ptEndLineE, 0)))
                        {
                            m_stTracks[iIndex].bEvent = false;
                            m_stTracks.erase(m_stTracks.begin() + iIndex);
                        }
                    }
                    else if ((m_iAllowedLeft == 1) && (m_iAllowedRight == 0))
                    {
                        if ((m_bCheckRight && m_pTracker->checkIntersection(stTrack, static_cast<float>(m_iWidthFrame), static_cast<float>(m_iHeightFrame), 
                                                                            RoadLine(m_ptRightLineB, m_ptRightLineE, 0))) 
                            || (m_bCheckEnd && m_pTracker->checkIntersection(stTrack, static_cast<float>(m_iWidthFrame), static_cast<float>(m_iHeightFrame), 
                                                                                RoadLine(m_ptEndLineB, m_ptEndLineE, 0))))
                        {
                            stBlowRedLights.push_back(m_stTracks[iIndex]);
                            m_stTracks.erase(m_stTracks.begin() + iIndex);
                        }
                    }
                    else if ((m_iAllowedLeft == 0) && (m_iAllowedRight == 1))
                    {
                        if ((m_bCheckLeft && m_pTracker->checkIntersection(stTrack, static_cast<float>(m_iWidthFrame), static_cast<float>(m_iHeightFrame), 
                                                                            RoadLine(m_ptLeftLineB, m_ptLeftLineE, 0)))
                            || (m_bCheckEnd && m_pTracker->checkIntersection(stTrack, static_cast<float>(m_iWidthFrame), static_cast<float>(m_iHeightFrame), 
                                                                                RoadLine(m_ptEndLineB, m_ptEndLineE, 0))))
                        {
                            stBlowRedLights.push_back(m_stTracks[iIndex]);
                            m_stTracks.erase(m_stTracks.begin() + iIndex);
                        }
                    }
                    else if ((m_iAllowedLeft == 1) && (m_iAllowedRight == 1))
                    {
                        if (m_bCheckEnd && m_pTracker->checkIntersection(stTrack, static_cast<float>(m_iWidthFrame), static_cast<float>(m_iHeightFrame), 
                                                                            RoadLine(m_ptEndLineB, m_ptEndLineE, 0)))
                        {
                            stBlowRedLights.push_back(m_stTracks[iIndex]);
                            m_stTracks.erase(m_stTracks.begin() + iIndex);
                        }
                    }
                }
                else // if find new track
                {
                    stOutputBlowRedLight_t stOutputBRL;
                    stOutputBRL.iTrackID = stTrack.m_ID;
                    stOutputBRL.riBox = stTrack.m_rect;
                    stOutputBRL.strType = stTrack.m_type;
                    if ((m_ptBeginLine.size() != 0) && (m_pTracker->checkIntersection(stTrack, static_cast<float>(m_iWidthFrame), static_cast<float>(m_iHeightFrame), 
                                                                                        RoadLine(m_ptBeginLineB, m_ptBeginLineE, 0))))
                    {
                        if (m_emStandardSignal == emLightState_t::Red)
                        {
                            cv::Mat mImageEvent = mImage.clone();
                            cv::rectangle(mImageEvent, stTrack.m_rect, cv::Scalar(0, 0, 255), 2);
                            stOutputBRL.mImageEvent = mImageEvent;
                            m_stTracks.push_back(stOutputBRL);
                        }
                    }
                }
            }
            else
            {
                const int iID = stTrack.m_ID;
                const auto p = find_if(m_stTracks.begin(), m_stTracks.end(), 
                                        [iID] (const stOutputBlowRedLight_t& st) { return (st.iTrackID == iID); });
                if (p != m_stTracks.end())
                {
                    int iIndex = distance(m_stTracks.begin(), p);
                    m_stTracks[iIndex].bOutOfFrame = true;
                }
            }
        }
    }

    return 0;
}

/*-------------------------------------------------------------------------------
Function: inline double BRL_GetBrighting(cv::Mat& mImage, cv::Rect riLight)
Purpose: 
Parameters: mImage, riLight
Return: 
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
inline double BRL_GetBrighting(cv::Mat& mImage, cv::Rect riLight)
{
    cv::Rect riSmallBox;
    riSmallBox.x = riLight.x + riLight.width / 4;
    riSmallBox.y = riLight.y + riLight.height / 4;
    riSmallBox.width = riLight.width / 2;
    riSmallBox.height = riLight.height / 2;

    cv::Mat mRoi = mImage(riSmallBox).clone();
    if (mRoi.empty())
    {
        return -1;
    }
    else
    {
        cv::Mat mGray;
        cv::cvtColor(mRoi, mGray, cv::COLOR_BGR2GRAY);
        return cv::mean(mGray)[0];
    }
}

/*-------------------------------------------------------------------------------
Function: emLightState_t BRL_GetLightState(cv::Mat& mImage, std::vector<std::vector<cv::Point>> ptLights)
Purpose: 
Parameters: mImage, ptLights
Return: 
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
emLightState_t BlowRedLight::BRL_GetLightState(cv::Mat& mImage, std::vector<std::vector<cv::Point>> ptLights)
{
    if (ptLights.size() == 0)
    {
        return emLightState_t::Undefined;
    }

    cv::Rect riRed, riYellow, riGreen;
    float fRed, fYellow, fGreen;
    if (ptLights.size() == 3)
    {
        riRed = cv::boundingRect(ptLights[0]);
        riYellow = cv::boundingRect(ptLights[1]);
        riGreen = cv::boundingRect(ptLights[2]);
        fRed = BRL_GetBrighting(mImage, riRed);
        fYellow = BRL_GetBrighting(mImage, riYellow);
        fGreen = BRL_GetBrighting(mImage, riGreen);
    }
    else if (ptLights.size() == 2)
    {
        riRed = cv::boundingRect(ptLights[0]);
        riYellow = cv::boundingRect(ptLights[1]);
        fRed = BRL_GetBrighting(mImage, riRed);
        fYellow = BRL_GetBrighting(mImage, riYellow);
        fGreen = -1;
    }
    else if (ptLights.size() == 1)
    {
        riRed = cv::boundingRect(ptLights[0]);
        fRed = BRL_GetBrighting(mImage, riRed);
        fYellow = -1;
        fGreen = -1;
    }

    if ((fRed < 80) && (fYellow < 80) && (fGreen < 80))
    {
        return emLightState_t::Off;
    }
    if ((fRed > fYellow) && (fRed > fGreen))
    {
        return emLightState_t::Red;
    }
    if ((fYellow > fRed) && (fYellow > fGreen))
    {
        return emLightState_t::Yellow;
    }
    if ((fGreen > fRed) && (fGreen > fYellow))
    {
        return emLightState_t::Green;
    }
    return emLightState_t::Undefined;
}