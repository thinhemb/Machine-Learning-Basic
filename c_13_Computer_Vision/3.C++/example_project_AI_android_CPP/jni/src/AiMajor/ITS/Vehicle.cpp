/********************************************************************************
<Copyright Notice>
Product: AI Camera
Module: AI Core
Version: 2.0
File: Vehicle.cpp
Author: QuangNTd
Created: 11-Jul-22
Modified:
    <Name>
    <Date>
    <Change>
Released: <Date>
Description: Implement of class Vehicle
Note: <Note>
********************************************************************************/

#include "Vehicle.hpp"

/*-------------------------------------------------------------------------------
Function: 
Purpose: 
Parameters: 
Return: 
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
Vehicle::Vehicle()
{
    m_pVehicleDetector = new ObjectDetector();
    m_pPlateDetector = new PlateDetector();
    m_pVehicleTracker = new ObjectTracking();
    m_pPlateTracker = new ObjectTracking();
    m_stPlateTracks.clear();
    m_stVehicleTracks.clear();
}

/*-------------------------------------------------------------------------------
Function: 
Purpose: 
Parameters: 
Return: 
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
Vehicle::~Vehicle()
{
    if (m_pVehicleDetector != nullptr) delete m_pVehicleDetector;
    if (m_pPlateDetector != nullptr) delete m_pPlateDetector;
    if (m_pVehicleTracker != nullptr) delete m_pVehicleDetector;
    if (m_pPlateTracker != nullptr) delete m_pPlateTracker;
    if (m_stOcrConfig != nullptr) delete m_stOcrConfig;
}

/*-------------------------------------------------------------------------------
Function: 
Purpose: 
Parameters: 
Return: 
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
int Vehicle::VHC_Init(stSettingVehicle_t stSettingVehicle)
{
    m_emTypeNation = stSettingVehicle.emTypeNation;
    m_strVehicles = stSettingVehicle.strVehicles;

    if (m_pVehicleDetector->initObjectDetector("snpe", "traffic", "DSP") != STATUS_SUCCESS)
    {
        std::cerr << "[Vehicle] Initialize Vehicle Detector failed." << std::endl;
        return STATUS_FAILED;
    }

    if (m_pPlateDetector->Init_PlateDetector(m_emTypeNation) != STATUS_SUCCESS)
    {
        std::cerr << "[Vehicle] Initialize PlateDetector failed." << std::endl;
        return STATUS_FAILED;
    }

    m_stOcrConfig = ITS_InitOcr(m_emTypeNation);

    std::cout << "[Vehicle] Initialize ANPR object successfully." << std::endl;

    return STATUS_SUCCESS;
}

/*-------------------------------------------------------------------------------
Function: 
Purpose: 
Parameters: 
Return: 
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
int Vehicle::VHC_Exe(cv::Mat& mImage, cv::Mat& mImage4k, std::vector<stVehicleInfor_t>& stVehicleInfors)
{
    stVehicleInfors.clear();
    if (mImage.empty() || mImage4k.empty())
    {
        std::cerr << "[Vehicle] Input image invalid." << std::endl;
        return STATUS_FAILED;
    }
    else
    {
        int iCols = mImage.cols;
        int iRows = mImage.rows;
        int iCols4k = mImage4k.cols;
        int iRows4k = mImage4k.rows;

        // detect vehicle
        std::vector<ObjectTrace> stVehicles;
        m_pVehicleDetector->executeObjectDetector(mImage, stVehicles, THRESHOLD_VEHICLE);
        // tracking vehicle
        std::vector<TrackingTrace> stVehicleTracks;
        m_pVehicleTracker->process(stVehicles, stVehicleTracks);
        std::cout << "[Vehicle] Tracking " << stVehicleTracks.size() << " vehicle" << std::endl;

        // detect plate
        std::vector<ObjectTrace> stPlates;
        m_pPlateDetector->Exe_PlateDetector(mImage, stPlates);
        // tracking plate
        std::vector<TrackingTrace> stPlateTracks;
        ITS_PlateTracking(m_pPlateTracker, m_emTypeNation, stPlates, stPlateTracks);
        std::cout << "[Vehicle] Tracking " << stPlateTracks.size() << " plate" << std::endl;

        // stVehicleTracks.insert(stVehicleTracks.end(), stPlateTracks.begin(), stPlateTracks.end());
        // std::cout << "[Vehicle] Merge tracking " << stVehicleTracks.size() << " object" << std::endl;

        // vehicle track
        // ITS_DeleteLoseTrack<std::vector<stVehicleInfor_t>>(m_stVehicleTracks, stVehicleTracks);
        for (auto it = m_stVehicleTracks.begin(); it != m_stVehicleTracks.end(); )
        {
            const int iID = (*it).iTrackID;
            const auto p = find_if(stVehicleTracks.begin(), stVehicleTracks.end(),
                                    [iID] (const TrackingTrace& a) { return (a.m_ID == iID); });

            if (p == stVehicleTracks.end() && it != m_stVehicleTracks.end())
            {
                it = m_stVehicleTracks.erase(it);
            }
            else
            {
                it++;
            }
        }

        for (auto stTrack : stVehicleTracks)
        {
            if (stTrack.isOutOfFrame == false)
            {
                const int iID = stTrack.m_ID;
                const auto p = find_if(m_stVehicleTracks.begin(), m_stVehicleTracks.end(),
                                        [iID] (const stVehicleInfor_t& a) { return (a.iTrackID == iID); });

                if (p != m_stVehicleTracks.end())
                {
                    int iIndex = distance(m_stVehicleTracks.begin(), p);
                    m_stVehicleTracks[iIndex].riBoxVehicle = stTrack.m_rect;
                    m_stVehicleTracks[iIndex].bOutOfFrame = false;
                    // m_stVehicleTracks[iIndex].stPlateInfor.bEvent = false;
                    if (VHC_CheckOutside(stTrack, iCols, iRows))
                    {
                        m_stVehicleTracks[iIndex].uiCountGoOut += 1;
                    }
                }
                else
                {
                    stVehicleInfor_t stVehicle;
                    stVehicle.riBoxVehicle = stTrack.m_rect;
                    stVehicle.iTrackID = stTrack.m_ID;
                    stVehicle.strTypeVehicle = stTrack.m_type;
                    stVehicle.bOutOfFrame = false;
                    m_stVehicleTracks.push_back(stVehicle);
                }
            }
            else
            {
                const int iID = stTrack.m_ID;
                const auto p = find_if(m_stVehicleTracks.begin(), m_stVehicleTracks.end(),
                                        [iID] (const stVehicleInfor_t& a) { return (a.iTrackID == iID); });

                if (p != m_stVehicleTracks.end())
                {
                    int iIndex = distance(m_stVehicleTracks.begin(), p);
                    m_stVehicleTracks[iIndex].bOutOfFrame = true;
                }
            }
        }

        // delete lose track
        // ITS_DeleteLoseTrack<std::vector<stPlateInfor_t>>(m_stPlateTracks, stPlateTracks);
        for (auto it = m_stPlateTracks.begin(); it != m_stPlateTracks.end(); )
        {
            const int iID = (*it).iTrackID;
            const auto p = find_if(stPlateTracks.begin(), stPlateTracks.end(),
                                    [iID] (const TrackingTrace& a) { return (a.m_ID == iID); });

            if (p == stPlateTracks.end() && it != m_stPlateTracks.end())
            {
                it = m_stPlateTracks.erase(it);
            }
            else
            {
                it++;
            }
        }

        for (auto stTrack : stPlateTracks)
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

                    if (m_stPlateTracks[iIndex].bEvent)
                    {
                        VHC_MappingVehicle(m_stPlateTracks[iIndex], m_stVehicleTracks);
                    }
                    else
                    {
                        VHC_UpdateVehicle(m_stPlateTracks[iIndex], m_stVehicleTracks);
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

                    if (stPlate.bEvent)
                    {
                        VHC_MappingVehicle(stPlate, m_stVehicleTracks);
                    }
                    else
                    {
                        VHC_UpdateVehicle(stPlate, m_stVehicleTracks);
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

        for (auto& stVehicle : m_stVehicleTracks)
        {
            if (stVehicle.bOutOfFrame == false)
            {
                stVehicle.bEvent = false;
                if (stVehicle.uiCountGoOut == 1 && stVehicle.bHavePlate == false)
                {
                    stVehicle.bEvent = true;
                    stVehicle.uiCountGoOut += 1;
                }

                if (stVehicle.stPlateInfor.bEvent)
                {
                    const int iPlateID = stVehicle.stPlateInfor.iTrackID;
                    const auto p = find_if(m_stPlateTracks.begin(), m_stPlateTracks.end(),
                                            [iPlateID] (const stPlateInfor_t& a) {return (a.iTrackID == iPlateID); });
                    if (p != m_stPlateTracks.end())
                    {
                        int iIndex = distance(m_stPlateTracks.begin(), p);
                        if (m_stPlateTracks[iIndex].bOutOfFrame == false)
                        {
                            stVehicle.bEvent = true;
                            stVehicle.stPlateInfor = m_stPlateTracks[iIndex];
                        }
                        else
                        {
                            stVehicle.bEvent = false;
                            stPlateInfor_t stPlateEmpty;
                            stVehicle.stPlateInfor = stPlateEmpty;
                        }
                    }
                }
                stVehicleInfors.push_back(stVehicle);
            }
        }
    }

    return STATUS_SUCCESS;
}

/*-------------------------------------------------------------------------------
Function: 
Purpose: 
Parameters: 
Return: 
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
void Vehicle::VHC_MappingVehicle(stPlateInfor_t& stPlate, std::vector<stVehicleInfor_t>& stVehicleTracks)
{
    cv::Point2f ptCenterBox = cv::Point2f(stPlate.riBoxPlate.x + (float)stPlate.riBoxPlate.width / 2,
                                            stPlate.riBoxPlate.y + (float)stPlate.riBoxPlate.height / 2);

    double dMaxDistance = 0;
    cv::Rect2i riBox;
    std::string strType;

    for (auto& stVehicle : stVehicleTracks)
    {
        if (stVehicle.bOutOfFrame == false && stVehicle.bEvent == false)
        {
            std::vector<cv::Point2f> ptAreas{cv::Point2f(stVehicle.riBoxVehicle.x, (float)stVehicle.riBoxVehicle.y + stVehicle.riBoxVehicle.height / 2),
                                            cv::Point2f(stVehicle.riBoxVehicle.x + stVehicle.riBoxVehicle.width, stVehicle.riBoxVehicle.y + stVehicle.riBoxVehicle.height / 2),
                                            cv::Point2f(stVehicle.riBoxVehicle.br()),
                                            cv::Point2f(stVehicle.riBoxVehicle.x, stVehicle.riBoxVehicle.y + stVehicle.riBoxVehicle.height)};
            
            double dDistance = cv::pointPolygonTest(ptAreas, ptCenterBox, true);
            if (dDistance >= dMaxDistance)
            {
                stVehicle.dDistance = dDistance;
                dMaxDistance = dDistance;
                riBox = stVehicle.riBoxVehicle;
                strType = stVehicle.strTypeVehicle;
            }
        }
    }

    if (dMaxDistance > 0)
    {
        const auto p = find_if(stVehicleTracks.begin(), stVehicleTracks.end(),
                                [dMaxDistance] (const stVehicleInfor_t& a) { return (a.dDistance == dMaxDistance); });
        if (p != stVehicleTracks.end())
        {
            int iIndex = distance(stVehicleTracks.begin(), p);
            stVehicleTracks[iIndex].riBoxVehicle = riBox;
            stVehicleTracks[iIndex].strTypeVehicle = strType;
            stVehicleTracks[iIndex].stPlateInfor = stPlate;
            stVehicleTracks[iIndex].bHavePlate = true;
        }
    }
    else
    {
        stVehicleInfor_t stVehicle;
        stVehicle.strTypeVehicle = "Unknown";
        stVehicle.stPlateInfor = stPlate;
        stVehicle.bHavePlate = true;
        stVehicleTracks.push_back(stVehicle);
    }
}

// /*-------------------------------------------------------------------------------
// Function: 
// Purpose: 
// Parameters: 
// Return: 
// Comments: 
// Modified: 
//     <Modified by>
//     <Date>
//     <Change>
// -------------------------------------------------------------------------------*/
// void Vehicle::VHC_EventVehicle(std::vector<stVehicleInfor_t>& stVehicleTracks, int iWidthImage, int iHeightImage)
// {
//     for (auto stVehicle : stVehicleTracks)
//     {
//         if (stVehicle.bOutOfFrame == false && stVehicle.bEvent == false)
//         {
//             // std::vector<cv::Point2f> ptAreas{cv::Point2f(iWidthImage * 0.2f, iHeightImage * 0.2f),
//             //                                 cv::Point2f(iWidthImage * 0.8f, iHeightImage * 0.2f),
//             //                                 cv::Point2f(iWidthImage * 0.8f, iHeightImage * 0.8f),
//             //                                 cv::Point2f(iWidthImage * 0.2f, iHeightImage * 0.8f)};

//             cv::Rect2f rfBox = cv::Rect2f(cv::Point2f(iWidthImage * 0.2f, iHeightImage * 0.2f),
//                                             cv::Point2f(iWidthImage * 0.8f, iHeightImage * 0.8f));

//             // 
//         }
//     }
// }

/*-------------------------------------------------------------------------------
Function: 
Purpose: 
Parameters: 
Return: 
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
bool Vehicle::VHC_CheckOutside(TrackingTrace& stTrack, int iWidthImage, int iHeightImage)
{
    if (stTrack.m_trace.size() < 2)
    {
        return false;
    }
    else
    {
        // std::vector<cv::Point2f> ptAreas{cv::Point2f(iWidthImage * 0.2f, iHeightImage * 0.2f),
        //                                 cv::Point2f(iWidthImage * 0.8f, iHeightImage * 0.2f),
        //                                 cv::Point2f(iWidthImage * 0.8f, iHeightImage * 0.8f),
        //                                 cv::Point2f(iWidthImage * 0.2f, iHeightImage * 0.8f)};

        cv::Rect2f rfBox = cv::Rect2f(cv::Point2f(iWidthImage * 0.2f, iHeightImage * 0.2f),
                                        cv::Point2f(iWidthImage * 0.8f, iHeightImage * 0.8f));

        cv::Point2f ptLastTrace = stTrack.m_trace[stTrack.m_trace.size() - 1];
        cv::Point2f ptPreTrace = stTrack.m_trace[stTrack.m_trace.size() - 2];

        if (rfBox.contains(ptLastTrace) == false && rfBox.contains(ptPreTrace) == true)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    
}

/*-------------------------------------------------------------------------------
Function: 
Purpose: 
Parameters: 
Return: 
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
void Vehicle::VHC_UpdateVehicle(stPlateInfor_t& stPlate, std::vector<stVehicleInfor_t>& stVehicleTracks)
{
    const int iPlateID = stPlate.iTrackID;
    const auto p = find_if(stVehicleTracks.begin(), stVehicleTracks.end(),
                            [iPlateID] (const stVehicleInfor_t& a) { return (a.stPlateInfor.iTrackID == iPlateID); });
    if (p != stVehicleTracks.end())
    {
        int iIndex = distance(stVehicleTracks.begin(), p);
        stVehicleTracks[iIndex].stPlateInfor = stPlate;
    }
}
