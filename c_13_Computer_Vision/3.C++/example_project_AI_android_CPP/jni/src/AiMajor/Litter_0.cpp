/********************************************************************************
<Copyright Notice>
Product: AI Camera
Module: AI Core
Version: 2.0
File: Litter.cpp
Author: QuangNTd
Created: 11-Jul-22
Modified:
    <Name>
    <Date>
    <Change>
Released: <Date>
Description: Implement of class Litter
Note: <Note>
********************************************************************************/

#include "Litter.hpp"

/*-------------------------------------------------------------------------------
Function: Litter()
Purpose: Construct a new Litter object
Parameters: 
Return: 
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
Litter::Litter()
{
    m_pObjectDetector = new ObjectDetector();
    m_pTrashDetector = new ObjectDetector();
    m_pObjectTracker = new ObjectTracking();
    m_pTrashTracker = new ObjectTracking();
}

/*-------------------------------------------------------------------------------
Function: ~Litter()
Purpose: Destroy the Litter object
Parameters: 
Return: 
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
Litter::~Litter()
{
    if (m_pObjectDetector != nullptr) delete m_pObjectDetector;
    if (m_pTrashDetector != nullptr) delete m_pTrashDetector;
    if (m_pObjectTracker != nullptr) delete m_pObjectTracker;
    if (m_pTrashTracker != nullptr) delete m_pTrashTracker;
}

/*-------------------------------------------------------------------------------
Function: int LT_Init()
Purpose: Initialize the Litter object
Parameters: 
Return: 
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
int Litter::LT_Init()
{
    if (m_pObjectDetector->initObjectDetector("snpe", "people", "DSP") != STATUS_SUCCESS)
    {
        // std::cerr << TagLitter << " Initialize Litter object failed." << std::endl;
        return -1;
    }

    if (m_pTrashDetector->initObjectDetector("snpe", "trash", "DSP") != STATUS_SUCCESS)
    {
        // std::cerr << TagLitter << " Initialize Litter object failed." << std::endl;
        return -1;
    }

    return 0;
}

/*-------------------------------------------------------------------------------
Function: int LT_Set(stSettingLitter_t stSetting)
Purpose: Set up the Litter object
Parameters: stSetting
Return: 
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
int Litter::LT_Set(stSettingLitter_t stSetting)
{
    m_ptRegion = stSetting.ptRegionSet;
    m_strObject = stSetting.strObjectSet;

    return 0;
}

/*-------------------------------------------------------------------------------
Function: int OP_Exe(cv::Mat& mImage, std::vector<stOutputLitter_t>& stLitters)
Purpose: Execute the Litter object
Parameters: mImage, stLitters
Return: 
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
int Litter::LT_Exe(cv::Mat& mImage, std::vector<stOutputLitter_t>& stLitters)
{
    stLitters.clear();
    if (mImage.empty())
    {
        // std::cerr << TagLitter << " Input invalid." << std::endl;
        return -1;
    }
    else
    {
        // detect object
        std::vector<ObjectTrace> stObjects;
        if (m_pObjectDetector->executeObjectDetector(mImage, stObjects, THRESH_OBJECT) != STATUS_SUCCESS)
        {
            // std::cerr << TagLitter << " Execute Object Detector failed." << std::endl;
            return -1;
        }
        // tracking object
        std::vector<TrackingTrace> stObjectTracks;
        m_pObjectTracker->process(stObjects, stObjectTracks);
        // delete lose object track
        for (auto it = m_stObjectTracks.begin(); it != m_stObjectTracks.end(); )
        {
            const int iID = (*it).iTrackID;
            const auto p = find_if(stObjectTracks.begin(), stObjectTracks.end(), 
                                    [iID] (const TrackingTrace& st) { return (st.m_ID == iID); });
            if (p == stObjectTracks.end() && it != m_stObjectTracks.end())
            {
                it = m_stObjectTracks.erase(it);
            }
            else
            {
                it ++;
            }
        }

        // detect trash
        std::vector<ObjectTrace> stTrashs;
        if (m_pTrashDetector->executeObjectDetector(mImage, stTrashs, THRESH_TRASH) != STATUS_SUCCESS)
        {
            // std::cerr << TagLitter << " Execute Trash Detector failed." << std::endl;
            return -1;
        }
        // tracking trash
        std::vector<TrackingTrace> stTrashTracks;
        m_pTrashTracker->process(stObjects, stTrashTracks);
        // delete lose trash track
        for (auto it = m_stTrashTracks.begin(); it != m_stTrashTracks.end(); )
        {
            const int iID = (*it).iTrackID;
            const auto p = find_if(stTrashTracks.begin(), stTrashTracks.end(), 
                                    [iID] (const TrackingTrace& st) { return (st.m_ID == iID); });
            if (p == stTrashTracks.end() && it != m_stTrashTracks.end())
            {
                it = m_stTrashTracks.erase(it);
            }
            else
            {
                it ++;
            }
        }

        // update object
        for (auto& stObjectTrack : stObjectTracks)
        {
            if (stObjectTrack.isOutOfFrame == false)
            {
                const int iID = stObjectTrack.m_ID;
                const auto p = find_if(m_stObjectTracks.begin(), m_stObjectTracks.end(), 
                                        [iID] (const stOutputLitter_t& st) { return (st.iTrackID == iID); });
                if (p != m_stObjectTracks.end()) // if find track in list object tracked
                {
                    int iIndex = distance(m_stObjectTracks.begin(), p);
                    m_stObjectTracks[iIndex].riBox = stObjectTrack.m_rect;
                    m_stObjectTracks[iIndex].bOutOfFrame = false;
                    if (LT_bCheckInside(stObjectTrack, m_ptRegion))
                    {
                        m_stObjectTracks[iIndex].bInside = true;
                    }
                    else
                    {
                        m_stObjectTracks[iIndex].bInside = false;
                    }
                }
                else // if find new object track
                {
                    stOutputLitter_t stLitter;
                    stLitter.iTrackID = stObjectTrack.m_ID;
                    stLitter.riBox = stObjectTrack.m_rect;
                    stLitter.strType = stObjectTrack.m_type;
                    if (LT_bCheckInside(stObjectTrack, m_ptRegion))
                    {
                        stLitter.bInside = true;
                    }
                    else
                    {
                        stLitter.bInside = false;
                    }
                    m_stObjectTracks.push_back(stLitter);
                }
            }
            else
            {
                const int iID = stObjectTrack.m_ID;
                const auto p = find_if(m_stObjectTracks.begin(), m_stObjectTracks.end(), 
                                        [iID] (const stOutputLitter_t& st) { return (st.iTrackID == iID); });
                if (p != m_stObjectTracks.end())
                {
                    int iIndex = distance(m_stObjectTracks.begin(), p);
                    m_stObjectTracks[iIndex].bOutOfFrame = true;
                }
            }
        }

        // update trash
        for (auto& stTrashTrack : stTrashTracks)
        {
            if (stTrashTrack.isOutOfFrame == false)
            {
                const int iID = stTrashTrack.m_ID;
                const auto p = find_if(m_stTrashTracks.begin(), m_stTrashTracks.end(), 
                                        [iID] (const stTrash_t& st) { return (st.iTrackID == iID); });
                if (p != m_stTrashTracks.end()) // if find track in list trash tracked
                {
                    int iIndex = distance(m_stTrashTracks.begin(), p);
                    m_stTrashTracks[iIndex].riBox = stTrashTrack.m_rect;
                    m_stTrashTracks[iIndex].bOutOfFrame = false;

                    stTrash_t stTrash = m_stTrashTracks[iIndex];
                    const int iTrashID = stTrash.iOwnID;
                    const auto pObject = find_if(m_stObjectTracks.begin(), m_stObjectTracks.end(), 
                                                [iTrashID] (const stOutputLitter_t& st) { return (st.iTrackID == iTrashID); });
                    int iIndexObject = distance(m_stObjectTracks.begin(), pObject);

                    if (stTrash.emState == emTrashState_t::Hold)
                    {
                        if (m_stObjectTracks[iIndexObject].bInside)
                        {
                            if (LT_bCheckState(stTrashTrack, m_stObjectTracks[iIndexObject]))
                            {
                                stTrash.emState = emTrashState_t::Hold;
                            }
                            else
                            {
                                stTrash.emState = emTrashState_t::ThrowedIn;
                                m_stObjectTracks[iIndexObject].stTrash = stTrash;
                                stLitters.push_back(m_stObjectTracks[iIndexObject]);
                            }
                        }
                        else
                        {
                            if (LT_bCheckState(stTrashTrack, m_stObjectTracks[iIndexObject]))
                            {
                                stTrash.emState = emTrashState_t::Hold;
                            }
                            else
                            {
                                stTrash.emState = emTrashState_t::ThrowedOut;
                            }
                        }
                    }
                }
                else // if find new trash track
                {
                    stTrash_t stTrash;
                    stTrash.iTrackID = stTrashTrack.m_ID;
                    stTrash.riBox = stTrashTrack.m_rect;
                    stTrash.strType = stTrashTrack.m_type;
                    
                    for (auto stObject : m_stObjectTracks)
                    {
                        if (LT_bCheckState(stTrashTrack, stObject))
                        {
                            stTrash.emState = emTrashState_t::Hold;
                            stTrash.iOwnID = stObject.iTrackID;
                        }
                    }

                    m_stTrashTracks.push_back(stTrash);
                }
            }
            else
            {
                const int iID = stTrashTrack.m_ID;
                const auto p = find_if(m_stTrashTracks.begin(), m_stTrashTracks.end(), 
                                        [iID] (const stTrash_t& st) { return (st.iTrackID == iID); });
                if (p != m_stTrashTracks.end())
                {
                    int iIndex = distance(m_stTrashTracks.begin(), p);
                    m_stTrashTracks[iIndex].bOutOfFrame = true;
                }
            }
        }

    }

    return 0;
}

/*-------------------------------------------------------------------------------
Function: bool LT_bCheckInside(TrackingTrace& stTrack, std::vector<cv::Point> ptArea)
Purpose: 
Parameters: stTrack, ptArea
Return: 
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
bool Litter::LT_bCheckInside(TrackingTrace& stTrack, std::vector<cv::Point> ptArea)
{
    if (ptArea.size() == 0)
    {
        return false;
    }

    cv::Point2f ptCenterBox1 = cv::Point2f(stTrack.m_rect.x, stTrack.m_rect.y);
    cv::Point2f ptCenterBox2 = cv::Point2f(stTrack.m_rect.x + stTrack.m_rect.width, stTrack.m_rect.y);

    if ((cv::pointPolygonTest(ptArea, ptCenterBox1, false) >= 0) || (cv::pointPolygonTest(ptArea, ptCenterBox2, false) >= 0))
    {
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------------
Function: bool LT_bCheckState(TrackingTrace& stTrashTrack, stOutputLitter_t& stLitter)
Purpose: 
Parameters: stTrashTrack, stLitter
Return: 
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
bool Litter::LT_bCheckState(TrackingTrace& stTrashTrack, stOutputLitter_t& stLitter)
{
    cv::Point2f pt1 = cv::Point2f(stLitter.riBox.x, stLitter.riBox.y);
    cv::Point2f pt2 = cv::Point2f(stLitter.riBox.x + stLitter.riBox.width, stLitter.riBox.y);
    cv::Point2f pt3 = cv::Point2f(stLitter.riBox.x + stLitter.riBox.width, stLitter.riBox.y + stLitter.riBox.height);
    cv::Point2f pt4 = cv::Point2f(stLitter.riBox.x, stLitter.riBox.y + stLitter.riBox.height);

    std::vector<cv::Point2f> ptArea = {pt1, pt2, pt3, pt4};

    cv::Point2f ptCenterBox1 = cv::Point2f(stTrashTrack.m_rect.x, stTrashTrack.m_rect.y);
    cv::Point2f ptCenterBox2 = cv::Point2f(stTrashTrack.m_rect.x + stTrashTrack.m_rect.width, stTrashTrack.m_rect.y);

    if ((cv::pointPolygonTest(ptArea, ptCenterBox1, false) >= 0) || (cv::pointPolygonTest(ptArea, ptCenterBox2, false) >= 0))
    {
        return true;
    }
    return false;
}