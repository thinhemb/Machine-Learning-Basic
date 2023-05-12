/********************************************************************************
<Copyright Notice>
Product: AI Camera
Module: AI Core
Version: 2.0
File: PlateDetector.hpp
Author: QuangNTd
Created: 11-Jul-22
Modified:
    <Name>
    <Date>
    <Change>
Released: <Date>
Description: Header of class PlateDetector
Note: <Note>
********************************************************************************/

#ifndef PlateDetector_hpp
#define PlateDetector_hpp

#include "ObjectDetector.hpp"
#include "ITScommon.hpp"

#define THRESHOLD_PLATE 0.35

class PlateDetector
{
    public:
        /**
         * @brief Construct a new Plate Detector object
         * 
         */
        PlateDetector();

        /**
         * @brief Destroy the Plate Detector object
         * 
         */
        ~PlateDetector();

        /**
         * @brief Initialize the Plate Detector object
         * 
         * @param emNation 
         * @return int 
         */
        int Init_PlateDetector(emNation_t emTypeNation);

        /**
         * @brief Execute the Plate Detector object
         * 
         * @param mImage 
         * @param stObjects 
         * @return int 
         */
        int Exe_PlateDetector(cv::Mat& mImage, std::vector<ObjectTrace>& stObjects);

    private:
        ObjectDetector* m_pPlateDetector = nullptr;
        
};

#endif // PlateDetector_hpp