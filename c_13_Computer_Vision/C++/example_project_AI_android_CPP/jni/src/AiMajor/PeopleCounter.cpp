/********************************************************************************
<Copyright Notice>
Product: AI Box
Module: AI Core
Version: 2.0
File: PeopleCounter.cpp
Author: QuangNTd
Created: 12-Jul-22
Modified:
    <Name>
    <Date>
    <Change>
Released: <Date>
Description: Implement of class PeopleCounter
Note: <Note>
********************************************************************************/

#include "PeopleCounter.hpp"

/*-------------------------------------------------------------------------------
Function: PeopleCounter()
Purpose: Construct a new People Counter object
Parameters: 
Return: 
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
PeopleCounter::PeopleCounter()
{
    m_pDetector = new ObjectDetector();
    m_pTracker = new ObjectTracking();

    m_stPeopleTracks.clear();
}

/*-------------------------------------------------------------------------------
Function: ~PeopleCounter()
Purpose: Destroy the People Counter object
Parameters: 
Return: 
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
PeopleCounter::~PeopleCounter()
{
    if (m_pDetector != nullptr) delete m_pDetector;
    if (m_pTracker != nullptr) delete m_pTracker;
}

/*-------------------------------------------------------------------------------
Function: int PC_Init()
Purpose: Initialize the People Counter object
Parameters: 
Return: int
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
int PeopleCounter::PC_Init()
{
    if (m_pDetector->initObjectDetector("ncnn", "traffic", "CPU") != STATUS_SUCCESS)
    {
        std::cerr << "[PeopleCounter] Initialize PeopleCounter object failed." << std::endl;
        return STATUS_FAILED;
    }

    return STATUS_SUCCESS;
}

/*-------------------------------------------------------------------------------
Function: int PC_Exe(cv::Mat& mImage, std::vector<stPeople_t>& stPeoples)
Purpose: Execute the People Counter object
Parameters: mImage, stPeoples
Return: int
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
int PeopleCounter::PC_Exe(cv::Mat& mImage, std::vector<stPeople_t>& stPeoples)
{
    stPeoples.clear();
    if (mImage.empty())
    {
        std::cerr << "[PeopleCounter] Input invalid." << std::endl;
        return STATUS_FAILED;
    }
    else
    {
        // cv::Mat mCopyImage;
        // mImage.copyTo(mCopyImage);

        // detect
        std::vector<ObjectTrace> stObjects;
        if (m_pDetector->executeObjectDetector(mImage, stObjects, THRESHOLD_PEOPLE) != STATUS_SUCCESS)
        {
            std::cerr << "[PeopleCounter] Execute Detector failed." << std::endl;
            return STATUS_FAILED;
        }

        // track
        std::vector<TrackingTrace> stTracks;
        m_pTracker->process(stObjects, stTracks, 5.0);

        // delete object which is abandoned
        for (auto it = m_stPeopleTracks.begin(); it != m_stPeopleTracks.end(); )
        {
            const int iID = (*it).iTrackID;
            const auto p = find_if(stTracks.begin(), stTracks.end(),
                                    [iID] (const TrackingTrace& a) { return (a.m_ID == iID); } );
            if (p == stTracks.end() && it != m_stPeopleTracks.end())
            {
                it = m_stPeopleTracks.erase(it);
                m_iSub += 1; // 
            }
            else
            {
                it++;
            }
        }

        // process new track and update old track
        for (auto& stTrack : stTracks)
        {
            if (stTrack.isOutOfFrame == false)
            {
                // cv::rectangle(mCopyImage, stTrack.m_rect, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);
                // std::string text{std::to_string(stTrack.m_ID) + " - " + stTrack.m_type};
			    // cv::putText(mCopyImage, text, cv::Point(stTrack.m_rect.x, stTrack.m_rect.y), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 255), 1);

                int iID = stTrack.m_ID;
                const auto p = find_if(m_stPeopleTracks.begin(), m_stPeopleTracks.end(),
                                        [iID] (const stPeople_t& a) { return (a.iTrackID == iID); });
                if (p != m_stPeopleTracks.end()) // if find object in list tracked object
                {
                    int iIndex = distance(m_stPeopleTracks.begin(), p);

                    m_stPeopleTracks[iIndex].riBox = stTrack.m_rect;
                    m_stPeopleTracks[iIndex].bOutOfFrame = false;
                }
                else // if find new object
                {
                    stPeople_t stPeople;
                    stPeople.iTrackID = stTrack.m_ID;
                    stPeople.strLabel = stTrack.m_type;
                    stPeople.riBox = stTrack.m_rect;
                    m_stPeopleTracks.push_back(stPeople);
                    m_iSum += 1; // 
                }
            }
            else
            {
                int iID = stTrack.m_ID;
                const auto p = find_if(m_stPeopleTracks.begin(), m_stPeopleTracks.end(),
                                        [iID] (const stPeople_t& a) { return (a.iTrackID == iID); });
                if (p != m_stPeopleTracks.end())
                {
                    int iIndex = distance(m_stPeopleTracks.begin(), p);

                    m_stPeopleTracks[iIndex].bOutOfFrame = true;
                }
            }
        }

        // check event
        bEvent = false;
        if (m_stPeopleTracks.size() != 0)
        {
            if ((m_iSub > 0) || (m_iSum > 0))
            {
                bEvent = true;
            }
        }

        m_iSub = 0;
        m_iSum = 0;

        // output data
        for (auto out : m_stPeopleTracks) 
        {
            if(out.bOutOfFrame == false)
            {
                stPeoples.push_back(out);
            }
        }

        // mImage = mCopyImage;
    }

    return STATUS_SUCCESS;
}
