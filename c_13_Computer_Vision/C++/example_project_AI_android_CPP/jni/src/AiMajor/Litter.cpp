/********************************************************************************
<Copyright Notice>
Product: AI Camera
Module: AI Core
Version: 2.0
File: Litter.cpp
Author: QuangNTd
Created: 14-Jul-22
Modified:
    <Name> QuangNTd
    <Date> 28-Aug-22
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
        std::cout << "stObjects.size() = " << stObjects.size() << std::endl;

        // detect trash
        std::vector<ObjectTrace> stTrashs;
        if (m_pTrashDetector->executeObjectDetector(mImage, stTrashs, THRESH_TRASH) != STATUS_SUCCESS)
        {
            // std::cerr << TagLitter << " Execute Trash Detector failed." << std::endl;
            return -1;
        }
        std::cout << "stTrashs.size() = " << stTrashs.size() << std::endl;

        // tracking trash
        std::vector<TrackingTrace> stTrashTracks;
        m_pTrashTracker->process(stTrashs, stTrashTracks);
        // delete lose trash track
        for (auto it = m_stLitterTracks.begin(); it != m_stLitterTracks.end(); )
        {
            const int iID = (*it).stTrash.iTrackID;
            const auto p = find_if(stTrashTracks.begin(), stTrashTracks.end(), 
                                    [iID] (const TrackingTrace& st) { return (st.m_ID == iID); });
            if (p == stTrashTracks.end() && it != m_stLitterTracks.end())
            {
                it = m_stLitterTracks.erase(it);
            }
            else
            {
                it ++;
            }
        }

        // update trash
        for (auto& stTrashTrack : stTrashTracks)
        {
            if (stTrashTrack.isOutOfFrame == false)
            {
                const int iID = stTrashTrack.m_ID;
                const auto p = find_if(m_stLitterTracks.begin(), m_stLitterTracks.end(), 
                                        [iID] (const stOutputLitter_t& st) { return (st.stTrash.iTrackID == iID); });
                if (p != m_stLitterTracks.end()) // if find track in list trash tracked
                {
                    int iIndex = distance(m_stLitterTracks.begin(), p);
                    m_stLitterTracks[iIndex].stTrash.riBox = stTrashTrack.m_rect;
                    m_stLitterTracks[iIndex].stTrash.bOutOfFrame = false;
                    m_stLitterTracks[iIndex].bEvent = false;
                    if (m_stLitterTracks[iIndex].strType == "Unknown")
                    {
                        LT_vGetObject(m_stLitterTracks[iIndex], stObjects);
                        if (m_stLitterTracks[iIndex].strType == "Unknown")
                        {
                            m_stLitterTracks[iIndex].bEvent = false;
                        }
                        else
                        {
                            m_stLitterTracks[iIndex].bEvent = true;
                        }
                    }
                }
                else // if find new trash track
                {
                    if (LT_bCheckMove(stTrashTrack) == true)
                    {
                        stOutputLitter_t stLitter;
                        stLitter.stTrash.iTrackID = stTrashTrack.m_ID;
                        stLitter.stTrash.riBox = stTrashTrack.m_rect;
                        stLitter.stTrash.strType = stTrashTrack.m_type;

                        LT_vGetObject(stLitter, stObjects);

                        if (stLitter.strType == "Unknown")
                        {
                            stLitter.bEvent = false;
                        }
                        else
                        {
                            stLitter.bEvent = true;
                        }

                        m_stLitterTracks.push_back(stLitter);
                    }
                }
            }
            else
            {
                const int iID = stTrashTrack.m_ID;
                const auto p = find_if(m_stLitterTracks.begin(), m_stLitterTracks.end(), 
                                        [iID] (const stOutputLitter_t& st) { return (st.stTrash.iTrackID == iID); });
                if (p != m_stLitterTracks.end())
                {
                    int iIndex = distance(m_stLitterTracks.begin(), p);
                    m_stLitterTracks[iIndex].stTrash.bOutOfFrame = true;
                }
            }
        }

        std::cout << "m_stLitterTracks.size() = " << m_stLitterTracks.size() << std::endl;

        for (auto stLitter : m_stLitterTracks)
        {
            if (stLitter.stTrash.bOutOfFrame != true)
            {
                if (stLitter.bEvent == true)
                {
                    stLitters.push_back(stLitter);
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
Function: bool LT_bCheckMove(TrackingTrace& stTrack)
Purpose: 
Parameters: stTrack
Return: 
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
bool Litter::LT_bCheckMove(TrackingTrace& stTrack)
{
    int iMinSize = 2;
    if (stTrack.m_trace.size() < iMinSize)
    {
        return false;
    }

    cv::Point2f ptLastTrace = stTrack.m_trace[stTrack.m_trace.size() - 1];
    cv::Point2f ptPreTrace = stTrack.m_trace[stTrack.m_trace.size() - iMinSize];

    double dDistance = cv::norm(ptLastTrace - ptPreTrace);

    if (dDistance > THRESH_TRASH_MOVE)
    {
        return true;
    }

    return false;
}

/*-------------------------------------------------------------------------------
Function: void LT_vGetObject(stOutputLitter_t& stLitter, std::vector<ObjectTrace>& stObjects)
Purpose: 
Parameters: stLitter, stObjects
Return: 
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
void Litter::LT_vGetObject(stOutputLitter_t& stLitter, std::vector<ObjectTrace>& stObjects)
{
    cv::Point2f ptCenterBox = cv::Point2f(stLitter.stTrash.riBox.x + (float)stLitter.stTrash.riBox.width,
                                            stLitter.stTrash.riBox.y + (float)stLitter.stTrash.riBox.height);
    
    if (stObjects.size() == 0)
    {
        stLitter.strType = "Unknown";
    }

    stLitter.strType = "Unknown";

    double dMaxDistance = -20;

    for (int i = 0; i < stObjects.size(); i++)
    {
        std::vector<cv::Point2f> ptAreas{stObjects[i].rect.tl(), 
                                            cv::Point2f(stObjects[i].rect.x + stObjects[i].rect.width, stObjects[i].rect.y), 
                                            stObjects[i].rect.br(), 
                                            cv::Point2f(stObjects[i].rect.x, stObjects[i].rect.y + stObjects[i].rect.height)};
        
        double dDistance = cv::pointPolygonTest(ptAreas, ptCenterBox, true);
        if (dDistance >= dMaxDistance)
        {
            stLitter.riBox = stObjects[i].rect;
            stLitter.strType = stObjects[i].label;

            dMaxDistance = dDistance;
        }
    }
}