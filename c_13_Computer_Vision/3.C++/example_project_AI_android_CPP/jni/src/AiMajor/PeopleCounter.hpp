/********************************************************************************
<Copyright Notice>
Product: AI Box
Module: AI Core
Version: 2.0
File: PeopleCounter.hpp
Author: QuangNTd
Created: 12-Jul-22
Modified:
    <Name>
    <Date>
    <Change>
Released: <Date>
Description: Header of class PeopleCounter
Note: <Note>
********************************************************************************/

#ifndef PeopleCounter_hpp
#define PeopleCounter_hpp

#include "ObjectTracking.hpp"
#include "ObjectDetector.hpp"

#define THRESHOLD_PEOPLE 0.5

/**
 * Structure of people object.
 */
typedef struct stPeople
{
    int iTrackID = 0;
    std::string strLabel;
    cv::Rect riBox;
    bool bOutOfFrame = false;
} stPeople_t;

/**
 * A class that contains the functions of People Counter object
 */
class PeopleCounter
{
    public:
        /**
         * @brief Construct a new People Counter object
         * 
         */
        PeopleCounter();

        /**
         * @brief Destroy the People Counter object
         * 
         */
        ~PeopleCounter();

        /**
         * @brief Initialize the People Counter object
         * 
         * @return int: 0 if initialized successfully; -1, otherwise.
         */
        int PC_Init();

        /**
         * @brief Execute the People Counter object
         * 
         * @param mImage Input image format cv::Mat
         * @param[in,out] stPeoples A vector of stPeople_t that will hold the output data
         * @return int: 0 if executed successfully; -1, otherwise.
         */
        int PC_Exe(cv::Mat& mImage, std::vector<stPeople_t>& stPeoples);

        /**
         * @brief boolean: true if had new event; false, otherwise.
         * 
         */
        bool bEvent = false;
    private:
        ObjectDetector* m_pDetector = nullptr;
        ObjectTracking* m_pTracker = nullptr;

        std::vector<stPeople_t> m_stPeopleTracks;
        int m_iSub = 0;
        int m_iSum = 0;
};

#endif // PeopleCounter_hpp
