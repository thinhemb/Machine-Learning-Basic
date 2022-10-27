/********************************************************************************
<Copyright Notice>
Product: AI Camera
Module: AI Core
Version: 2.0
File: PlateDetector.cpp
Author: QuangNTd
Created: 11-Jul-22
Modified:
    <Name>
    <Date>
    <Change>
Released: <Date>
Description: Implement of class PlateDetector
Note: <Note>
********************************************************************************/

#include "PlateDetector.hpp"

/*-------------------------------------------------------------------------------
Function: PlateDetector
Purpose: Construct a new Plate Detector object
Parameters: 
Return: 
Comments: <Any addition comments>
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
PlateDetector::PlateDetector()
{
    m_pPlateDetector = new ObjectDetector();
}

/*-------------------------------------------------------------------------------
Function: ~PlateDetector()
Purpose: Destroy the Plate Detector object
Parameters: 
Return: 
Comments: <Any addition comments>
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
PlateDetector::~PlateDetector()
{
    if (m_pPlateDetector != nullptr)
        delete m_pPlateDetector;
}

/*-------------------------------------------------------------------------------
Function: int Init_PlateDetector(emNation_t emNation)
Purpose: Initialize the Plate Detector object
Parameters: emNation
Return: int
Comments: <Any addition comments>
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
int PlateDetector::Init_PlateDetector(emNation_t emTypeNation)
{
    switch (emTypeNation)
    {
        case emNation_t::VietNam:
        {
            if (m_pPlateDetector->initObjectDetector("snpe", "AnprDetect", "DSP") != STATUS_SUCCESS)
            {
                std::cerr << "[PlateDetector] Initialize PlateDetector VietNam failed." << std::endl;
                return STATUS_FAILED;
            }
            break;
        }
        case emNation_t::USA:
        {
            if (m_pPlateDetector->initObjectDetector("snpe", "AnprDetect_US", "DSP") != STATUS_SUCCESS)
            {
                std::cerr << "[PlateDetector] Initialize PlateDetector USA failed." << std::endl;
                return STATUS_FAILED;
            }
            break;
        }
        case emNation_t::Malaysia:
        {
            if (m_pPlateDetector->initObjectDetector("snpe", "AnprDetect_Malay", "DSP") != STATUS_SUCCESS)
            {
                std::cerr << "[PlateDetector] Initialize PlateDetector Malaysia failed." << std::endl;
                return STATUS_FAILED;
            }
            break;
        }
        default:
        {
            std::cerr << "[PlateDetector] Plate of Nation is unsupported." << std::endl;
            break;
        }
    }
    std::cout << "[PlateDetector] Initialize PlateDetector successfully." << std::endl;

    return STATUS_SUCCESS;
}

/*-------------------------------------------------------------------------------
Function: int Exe_PlateDetector(cv::Mat& mImage, std::vector<ObjectTrace>& stObjects)
Purpose: Execute the Plate Detector object
Parameters: mImage, stObjects
Return: int
Comments: <Any addition comments>
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
int PlateDetector::Exe_PlateDetector(cv::Mat& mImage, std::vector<ObjectTrace>& stObjects)
{
    if (mImage.empty())
    {
        std::cerr << "Input image invalid." << std::endl;
        return STATUS_FAILED;
    }
    else
    {
        if (m_pPlateDetector->executeObjectDetector(mImage, stObjects, THRESHOLD_PLATE) != STATUS_SUCCESS)
        {
            std::cerr << "[PlateDetector] Execute PlateDetector failed." << std::endl;
            return STATUS_FAILED;
        }

        for (size_t i = 0; i < stObjects.size(); i++)
        {
            stObjects[i].rect.x -= stObjects[i].rect.width * 0.1;
            if (stObjects[i].rect.x < 0)
            {
                stObjects[i].rect.x = 0;
            }

            stObjects[i].rect.y -= stObjects[i].rect.height * 0.1;
            if (stObjects[i].rect.y < 0)
            {
                stObjects[i].rect.y = 0;
            }

            stObjects[i].rect.width += stObjects[i].rect.width * 0.3;
            if ((stObjects[i].rect.x + stObjects[i].rect.width) > mImage.cols)
            {
                stObjects[i].rect.width = mImage.cols - stObjects[i].rect.x;
            }

            stObjects[i].rect.height += stObjects[i].rect.height * 0.3;
            if ((stObjects[i].rect.y + stObjects[i].rect.height) > mImage.rows)
            {
                stObjects[i].rect.height = mImage.rows - stObjects[i].rect.y;
            }
        }
    }

    return STATUS_SUCCESS;
}
