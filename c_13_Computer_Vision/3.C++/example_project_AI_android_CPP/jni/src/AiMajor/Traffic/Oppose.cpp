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
Description: Implement of class Oppose
Note: <Note>
********************************************************************************/

#include "Oppose.hpp"

/*-------------------------------------------------------------------------------
Function: Oppose()
Purpose: Construct a new Oppose object
Parameters: 
Return: 
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
Oppose::Oppose()
{
    m_pDetector = new ObjectDetector();
    m_pTracker = new ObjectTracking();
}

/*-------------------------------------------------------------------------------
Function: ~Oppose()
Purpose: Destroy the Oppose object
Parameters: 
Return: 
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
Oppose::~Oppose()
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
Function: int OP_Init()
Purpose: Initialize the Oppose object
Parameters: 
Return: 
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
int Oppose::OP_Init()
{
    if (m_pDetector->initObjectDetector("snpe", "traffic", "DSP") != STATUS_SUCCESS)
    {
        // std::cerr << TagOppose << " Initialize Oppose object failed." << std::endl;
        return -1;
    }

    return 0;
}

/*-------------------------------------------------------------------------------
Function: int OP_Set(stSettingOppose_t stSetting)
Purpose: Set up the Oppose object
Parameters: stSetting
Return: 
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
int Oppose::OP_Set(stSettingOppose_t stSetting)
{
    m_ptRegion = stSetting.ptRegionSet;
    m_stDirection = stSetting.stDirectionSet;
    m_strVehicle = stSetting.strVehicleSet;

    if (m_stDirection.ptHead.y < m_stDirection.ptTail.y)
    {
        bLess = true;
    }
    else
    {
        bLess = false;
    }

    return 0;
}

/*-------------------------------------------------------------------------------
Function: int OP_Exe(cv::Mat& mImage, std::vector<stOutputOppose_t>& stOpposes)
Purpose: Execute the Oppose object
Parameters: mImage, stOpposes
Return: 
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
int Oppose::OP_Exe(cv::Mat& mImage, std::vector<stOutputOppose_t>& stOpposes)
{
    stOpposes.clear();
    if (mImage.empty())
    {
        // std::cerr << OPPOSE << " Input invalid." << std::endl;
        return -1;
    }
    else
    {
        // detect
        std::vector<ObjectTrace> stObjects;
        if (m_pDetector->executeObjectDetector(mImage, stObjects, THRESH_OPPOSE) != STATUS_SUCCESS)
        {
            // std::cerr << TagOppose << " Execute Detector failed." << std::endl;
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
                if (m_strVehicle.size() == 0)
                {
                    int iID = stTrack.m_ID;
                    const auto p = find_if(m_stTracks.begin(), m_stTracks.end(), 
                                            [iID] (const stOutputOppose_t& st) { return (st.iTrackID == iID); });
                    if (p != m_stTracks.end()) // if find track in list tracked
                    {
                        int iIndex = distance(m_stTracks.begin(), p);
                        m_stTracks[iIndex].riBox = stTrack.m_rect;
                        m_stTracks[iIndex].bOutOfFrame = false;
                        m_stTracks[iIndex].bEvent = false;

                        if (OP_bCheckLane(stTrack, m_ptRegion))
                        {
                            if (m_stTracks[iIndex].iCountCheck <= MAX_COUNT_CHECK)
                            {
                                float fDistance = OP_fCalDistanceTrace(stTrack);
                                if (bLess)
                                {
                                    if ((fDistance < 0) && ((fDistance + THRESH_TRACE) < 0))
                                    {
                                        m_stTracks[iIndex].iCountCheck += 1;
                                    }
                                    else
                                    {
                                        m_stTracks[iIndex].iCountCheck -= 1;
                                    }
                                }
                                else
                                {
                                    if ((fDistance > 0) && ((fDistance - THRESH_TRACE) > 0))
                                    {
                                        m_stTracks[iIndex].iCountCheck += 1;
                                    }
                                    else
                                    {
                                        m_stTracks[iIndex].iCountCheck -= 1;
                                    }
                                }
                            }
                            else if (m_stTracks[iIndex].iCountCheck == MAX_COUNT_CHECK + 1)
                            {
                                m_stTracks[iIndex].iCountCheck += 1;
                                m_stTracks[iIndex].bEvent = true;
                            }
                        }
                        else
                        {
                            m_stTracks.erase(m_stTracks.begin() + iIndex);
                        }
                    }
                    else // if find new track
                    {
                        if (OP_bCheckLane(stTrack, m_ptRegion))
                        {
                            stOutputOppose_t stOppose;
                            stOppose.iTrackID = stTrack.m_ID;
                            stOppose.riBox = stTrack.m_rect;
                            stOppose.strType = stTrack.m_type;
                            m_stTracks.push_back(stOppose);
                        }
                    }
                }
                else 
                {
                    if (find(m_strVehicle.begin(), m_strVehicle.end(), stTrack.m_type) != m_strVehicle.end())
                    {
                        int iID = stTrack.m_ID;
                        const auto p = find_if(m_stTracks.begin(), m_stTracks.end(), 
                                                [iID] (const stOutputOppose_t& st) { return (st.iTrackID == iID); });
                        if (p != m_stTracks.end()) // if find track in list tracked
                        {
                            int iIndex = distance(m_stTracks.begin(), p);
                            m_stTracks[iIndex].riBox = stTrack.m_rect;
                            m_stTracks[iIndex].bOutOfFrame = false;
                            m_stTracks[iIndex].bEvent = false;

                            if (OP_bCheckLane(stTrack, m_ptRegion))
                            {
                                if (m_stTracks[iIndex].iCountCheck <= MAX_COUNT_CHECK)
                                {
                                    float fDistance = OP_fCalDistanceTrace(stTrack);
                                    if (bLess)
                                    {
                                        if ((fDistance < 0) && ((fDistance + THRESH_TRACE) < 0))
                                        {
                                            m_stTracks[iIndex].iCountCheck += 1;
                                        }
                                        else
                                        {
                                            m_stTracks[iIndex].iCountCheck -= 1;
                                        }
                                    }
                                    else
                                    {
                                        if ((fDistance > 0) && ((fDistance - THRESH_TRACE) > 0))
                                        {
                                            m_stTracks[iIndex].iCountCheck += 1;
                                        }
                                        else
                                        {
                                            m_stTracks[iIndex].iCountCheck -= 1;
                                        }
                                    }
                                }
                                else if (m_stTracks[iIndex].iCountCheck == MAX_COUNT_CHECK + 1)
                                {
                                    m_stTracks[iIndex].iCountCheck += 1;
                                    m_stTracks[iIndex].bEvent = true;
                                }
                            }
                            else
                            {
                                m_stTracks.erase(m_stTracks.begin() + iIndex);
                            }
                        }
                        else // if find new track
                        {
                            if (OP_bCheckLane(stTrack, m_ptRegion))
                            {
                                stOutputOppose_t stOppose;
                                stOppose.iTrackID = stTrack.m_ID;
                                stOppose.riBox = stTrack.m_rect;
                                stOppose.strType = stTrack.m_type;
                                m_stTracks.push_back(stOppose);
                            }
                        }
                    }
                }
            }
            else
            {
                const int iID = stTrack.m_ID;
                const auto p = find_if(m_stTracks.begin(), m_stTracks.end(), 
                                        [iID] (const stOutputOppose_t& st) { return (st.iTrackID == iID); });
                if (p != m_stTracks.end())
                {
                    int iIndex = distance(m_stTracks.begin(), p);
                    m_stTracks[iIndex].bOutOfFrame = true;
                }
            }
        }

        // get output
        for (auto stTrack : m_stTracks)
        {
            if (stTrack.bOutOfFrame == false)
            {
                if (stTrack.bEvent)
                {
                    stOpposes.push_back(stTrack);
                }
            }
        }
    }

    return 0;
}

/*-------------------------------------------------------------------------------
Function: bool OP_bCheckLane(TrackingTrace stTrack, std::vector<cv::Point> ptArea)
Purpose: 
Parameters: stTrack, ptArea
Return: 
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
bool Oppose::OP_bCheckLane(TrackingTrace stTrack, std::vector<cv::Point> ptArea)
{
    if (ptArea.size() == 0)
    {
        return false;
    }
    else
    {
        cv::Point2f ptCenterBox = cv::Point2f(stTrack.m_rect.x + float(stTrack.m_rect.width / 2), 
                                                stTrack.m_rect.y + float(stTrack.m_rect.height / 2));
        if (cv::pointPolygonTest(ptArea, ptCenterBox, false) >= 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}

/*-------------------------------------------------------------------------------
Function: float OP_fCalDistanceTrace(TrackingTrace stTrack)
Purpose: 
Parameters: stTrack
Return: 
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
float Oppose::OP_fCalDistanceTrace(TrackingTrace stTrack)
{
    int iMinSize = 3;
    float fDistance;
    
    if (stTrack.m_trace.size() < iMinSize)
    {
        fDistance = 0;
    }
    else
    {
        cv::Point2f ptLastTrace = stTrack.m_trace[stTrack.m_trace.size() - 1];
        cv::Point2f ptPreTrace = stTrack.m_trace[stTrack.m_trace.size() - iMinSize];
        
        fDistance = ptLastTrace.y - ptPreTrace.y;
    }
    
    return fDistance;
}