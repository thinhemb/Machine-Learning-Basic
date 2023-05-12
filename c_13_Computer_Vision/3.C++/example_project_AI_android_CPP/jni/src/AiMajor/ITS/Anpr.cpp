/********************************************************************************
<Copyright Notice>
Product: AI Camera
Module: AI Core
Version: 2.0
File: Anpr.cpp
Author: QuangNTd
Created: 11-Jul-22
Modified:
    <Name>
    <Date>
    <Change>
Released: <Date>
Description: Implement of class Anpr
Note: <Note>
********************************************************************************/

#include "Anpr.hpp"

/*-------------------------------------------------------------------------------
Function: 
Purpose: 
Parameters: 
Return: 
Comments: <Any addition comments>
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
ANPR::ANPR()
{
    m_pPlateDetector = new PlateDetector();
    m_pTracker = new ObjectTracking();
    m_stOcrConfig = new stOcrConfig_t;

    m_stPlateTracks.clear();
}

/*-------------------------------------------------------------------------------
Function: 
Purpose: 
Parameters: 
Return: 
Comments: <Any addition comments>
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
ANPR::~ANPR()
{
    if (m_pPlateDetector != nullptr) delete m_pPlateDetector;
    if (m_pTracker != nullptr) delete m_pTracker;
    if (m_stOcrConfig != nullptr) delete m_stOcrConfig;
}

/*-------------------------------------------------------------------------------
Function: 
Purpose: 
Parameters: 
Return: 
Comments: <Any addition comments>
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
int ANPR::Init_ANPR(emNation_t emTypeNation)
{
    m_emTypeNation = emTypeNation;

    if (m_pPlateDetector->Init_PlateDetector(m_emTypeNation) != STATUS_SUCCESS)
    {
        std::cerr << "Init_PlateDetector failed." << std::endl;
        return STATUS_FAILED;
    }

    m_stOcrConfig = ITS_InitOcr(m_emTypeNation);

    std::cout << "[ANPR] Initialize ANPR object successfully." << std::endl;

    return STATUS_SUCCESS;
}

/*-------------------------------------------------------------------------------
Function: 
Purpose: 
Parameters: 
Return: 
Comments: <Any addition comments>
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
int ANPR::Exe_ANPR(cv::Mat& mImage, cv::Mat& mImage4k, std::vector<stPlateInfor_t>& stPlates)
{
    stPlates.clear();
    if (mImage.empty() || mImage4k.empty())
    {
        std::cerr << "[ANPR] Input image invalid." << std::endl;
        return STATUS_FAILED;
    }
    else
    {
        int iCols = mImage.cols;
        int iRows = mImage.rows;
        int iCols4k = mImage4k.cols;
        int iRows4k = mImage4k.rows;

        // detect plate
        std::vector<ObjectTrace> stObjects;
        m_pPlateDetector->Exe_PlateDetector(mImage, stObjects);

        // tracking
        std::vector<TrackingTrace> stTracks;
        ITS_PlateTracking(m_pTracker, m_emTypeNation, stObjects, stTracks);

        // delete object which is abandoned
        for (auto it = m_stPlateTracks.begin(); it != m_stPlateTracks.end(); )
        {
            const int iID = (*it).iTrackID;
            const auto p = find_if(stTracks.begin(), stTracks.end(),
                                    [iID] (const TrackingTrace& a) { return (a.m_ID == iID); });

            if (p == stTracks.end() && it != m_stPlateTracks.end())
            {
                it = m_stPlateTracks.erase(it);
            }
            else
            {
                it++;
            }
        }

        // process new track and update old track
        for (auto stTrack : stTracks)
        {
            if (stTrack.isOutOfFrame == false)
            {
                const int iID = stTrack.m_ID;
                const auto p = find_if(m_stPlateTracks.begin(), m_stPlateTracks.end(),
                                        [iID] (const stPlateInfor_t& a) { return (a.iTrackID == iID); });

                if (p != m_stPlateTracks.end()) // if find plate in list tracked plate
                {
                    int iIndex = distance(m_stPlateTracks.begin(), p);
                    m_stPlateTracks[iIndex].riBoxPlate = stTrack.m_rect;
                    m_stPlateTracks[iIndex].bOutOfFrame = false;
                    
                    if ((m_stPlateTracks[iIndex].strLicense == "Unknown" || m_stPlateTracks[iIndex].bDefectPlate)
                        && m_stPlateTracks[iIndex].uiCountUnknown <= MAX_COUNT_UNKNOWN)
                    {
                        cv::Rect2f rfBox4k = ITS_GetBox4k(stTrack.m_rect, iCols4k, iRows4k);
                        cv::Mat mCropPlate4k = mImage4k(rfBox4k).clone();
                        cv::Mat mCropPlate = mImage(stTrack.m_rect).clone();
                        float fConfidence = 0.f;

                        m_stPlateTracks[iIndex].mCropPlate = mCropPlate;

                        if (ITS_CheckPlate(m_stPlateTracks[iIndex], iCols, iRows))
                        {
                            m_stPlateTracks[iIndex].bEvent = true;
                            m_stPlateTracks[iIndex].strLicense = ITS_ReadText(m_stOcrConfig, mCropPlate4k, m_emTypeNation, stTrack.m_type, fConfidence);
                            if (!isnan(fConfidence))
                            {
                                m_stPlateTracks[iIndex].fScore = fConfidence;
                            }
                            else
                            {
                                m_stPlateTracks[iIndex].fScore = 0;
                            }

                            if (m_stPlateTracks[iIndex].strLicense == "Unknown")
                            {
                                m_stPlateTracks[iIndex].uiCountUnknown ++;
                                m_stPlateTracks[iIndex].bEvent = false;
                            }
                            else if (m_stPlateTracks[iIndex].strLicense.size() < 7 || m_stPlateTracks[iIndex].strLicense.size() > 10)
                            {
                                m_stPlateTracks[iIndex].uiCountUnknown++;
                                m_stPlateTracks[iIndex].bEvent = false;
                            }
                            else
                            {
                                m_stPlateTracks[iIndex].bDefectPlate = false;
                            }
                        }
                        else
                        {
                            m_stPlateTracks[iIndex].strLicense = "Unknown";
                            m_stPlateTracks[iIndex].fScore = 0;
                            m_stPlateTracks[iIndex].bEvent = false;
                        }
                    }
                    else if (m_stPlateTracks[iIndex].uiCountUnknown == (MAX_COUNT_UNKNOWN + 1))
                    {
                        m_stPlateTracks[iIndex].uiCountUnknown++;
                        m_stPlateTracks[iIndex].bDefectPlate = false;
                        m_stPlateTracks[iIndex].bEvent = true;
                    }
                    else
                    {
                        m_stPlateTracks[iIndex].bEvent = false;
                    }
                }
                else // if find new plate
                {
                    cv::Rect2f rfBox4k = ITS_GetBox4k(stTrack.m_rect, iCols4k, iRows4k);
                    cv::Mat mCropPlate4k = mImage4k(rfBox4k).clone();
                    cv::Mat mCropPlate = mImage(stTrack.m_rect).clone();
                    float fConfidence = 0.f;

                    stPlateInfor_t stPlate;
                    stPlate.mCropPlate = mCropPlate;
                    stPlate.iTrackID = stTrack.m_ID;
                    stPlate.riBoxPlate = stTrack.m_rect;
                    stPlate.strTypePlate = stTrack.m_type;
                    stPlate.bOutOfFrame = false;
                    if (ITS_CheckPlate(stPlate, iCols, iRows))
                    {
                        stPlate.bEvent = true;
                        stPlate.strLicense = ITS_ReadText(m_stOcrConfig, mCropPlate4k, m_emTypeNation, stTrack.m_type, fConfidence);
                        if (!isnan(fConfidence))
                        {
                            stPlate.fScore = fConfidence;
                        }
                        else
                        {
                            stPlate.fScore = 0;
                        }

                        if (stPlate.strLicense.size() < 7 || stPlate.strLicense.size() > 10)
                        {
                            stPlate.bDefectPlate = true;
                            stPlate.uiCountUnknown = 1;
                        }
                    }
                    else
                    {
                        stPlate.strLicense = "Unknown";
                        stPlate.fScore = 0;
                    }

                    if (stPlate.strLicense == "Unknown" || stPlate.bDefectPlate)
                    {
                        stPlate.bEvent = false;
                    }

                    m_stPlateTracks.push_back(stPlate);
                }
            }
            else
            {
                const int iID = stTrack.m_ID;
                const auto p = find_if(m_stPlateTracks.begin(), m_stPlateTracks.end(),
                                        [iID] (const stPlateInfor_t& a) { return (a.iTrackID == iID); });

                if (p != m_stPlateTracks.end())
                {
                    int iIndex = distance(m_stPlateTracks.begin(), p);
                    m_stPlateTracks[iIndex].bOutOfFrame = true;
                }
            }
        }

        // get list of output plates
        for (auto stPlate : m_stPlateTracks)
        {
            if (stPlate.bOutOfFrame != true)
            {
                stPlates.push_back(stPlate);
            }
        }
    }

    return STATUS_SUCCESS;
}