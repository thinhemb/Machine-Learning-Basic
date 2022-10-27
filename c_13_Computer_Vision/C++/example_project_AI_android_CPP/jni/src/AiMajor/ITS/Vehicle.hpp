/********************************************************************************
<Copyright Notice>
Product: AI Camera
Module: AI Core
Version: 2.0
File: Vehicle.hpp
Author: QuangNTd
Created: 14-Jul-22
Modified:
    <Name>
    <Date>
    <Change>
Released: <Date>
Description: Header of class Vehicle
Note: <Note>
********************************************************************************/

#ifndef Vehicle_hpp
#define Vehicle_hpp

#include "ITScommon.hpp"
#include "PlateDetector.hpp"

#define THRESHOLD_VEHICLE 0.4

class Vehicle
{
    public:
        /**
         * @brief Construct a new Vehicle object
         * 
         */
        Vehicle();

        /**
         * @brief Destroy the Vehicle object
         * 
         */
        ~Vehicle();

        /**
         * @brief 
         * 
         * @param stSettingVehicle 
         * @return int 
         */
        int VHC_Init(stSettingVehicle_t stSettingVehicle);

        /**
         * @brief 
         * 
         * @param mImage 
         * @param mImage4k 
         * @param stVehicles 
         * @return int 
         */
        int VHC_Exe(cv::Mat& mImage, cv::Mat& mImage4k, std::vector<stVehicleInfor_t>& stVehicleInfors);
    private:
        emNation_t m_emTypeNation;
        std::vector<std::string> m_strVehicles;

        ObjectDetector* m_pVehicleDetector = nullptr;
        PlateDetector* m_pPlateDetector = nullptr;
        ObjectTracking* m_pVehicleTracker = nullptr;
        ObjectTracking* m_pPlateTracker = nullptr;
        stOcrConfig_t* m_stOcrConfig = nullptr;

        std::vector<stPlateInfor_t> m_stPlateTracks;
        std::vector<stVehicleInfor_t> m_stVehicleTracks;
    private:

        /**
         * @brief 
         * 
         * @param stPlate 
         * @param stVehicleTracks 
         */
        void VHC_MappingVehicle(stPlateInfor_t& stPlate, std::vector<stVehicleInfor_t>& stVehicleTracks);

        // /**
        //  * @brief 
        //  * 
        //  * @param stVehicle 
        //  * @param iWidthImage 
        //  * @param iHeightImage 
        //  */
        // void VHC_EventVehicle(std::vector<stVehicleInfor_t>& stVehicleTracks, int iWidthImage, int iHeightImage);

        /**
         * @brief 
         * 
         * @param stTrack 
         * @param iWidthImage 
         * @param iHeightImage 
         * @return true 
         * @return false 
         */
        bool VHC_CheckOutside(TrackingTrace& stTrack, int iWidthImage, int iHeightImage);

        /**
         * @brief 
         * 
         * @param stPlate 
         * @param stVehicleTracks 
         */
        void VHC_UpdateVehicle(stPlateInfor_t& stPlate, std::vector<stVehicleInfor_t>& stVehicleTracks);
};

#endif // Vehicle_hpp