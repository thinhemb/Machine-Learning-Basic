/********************************************************************************
<Copyright Notice>
Product: AI Camera
Module: AI Core
Version: 2.0
File: Anpr.hpp
Author: QuangNTd
Created: 11-Jul-22
Modified:
    <Name>
    <Date>
    <Change>
Released: <Date>
Description: Header of class Anpr
Note: <Note>
********************************************************************************/

#ifndef Anpr_hpp
#define Anpr_hpp

#include "ITScommon.hpp"
#include "PlateDetector.hpp"

class ANPR
{
    public:
        /**
         * @brief Construct a new ANPR object
         * 
         */
        ANPR();

        /**
         * @brief Destroy the ANPR object
         * 
         */
        ~ANPR();

        /**
         * @brief 
         * 
         * @param emTypeNation 
         * @return int 
         */
        int Init_ANPR(emNation_t emTypeNation);

        /**
         * @brief 
         * 
         * @param mImage 
         * @param mImage4k 
         * @param stPlates 
         * @return int 
         */
        int Exe_ANPR(cv::Mat& mImage, cv::Mat& mImage4k, std::vector<stPlateInfor_t>& stPlates);
    private:
        emNation_t m_emTypeNation;

        PlateDetector* m_pPlateDetector = nullptr;
        ObjectTracking* m_pTracker = nullptr;
        stOcrConfig_t* m_stOcrConfig = nullptr;

        std::vector<stPlateInfor_t> m_stPlateTracks;
};

#endif // Anpr_hpp