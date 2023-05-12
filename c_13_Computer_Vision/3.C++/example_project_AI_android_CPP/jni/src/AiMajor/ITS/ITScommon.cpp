/********************************************************************************
<Copyright Notice>
Product: AI Camera
Module: AI Core
Version: 2.0
File: ITScommon.cpp
Author: QuangNTd
Created: 11-Jul-22
Modified:
    <Name>
    <Date>
    <Change>
Released: <Date>
Description: Implement of common functions for ITS
Note: <Note>
********************************************************************************/

#include "ITScommon.hpp"

/*-------------------------------------------------------------------------------
Function: stOcrConfig* ITS_InitOcr(emNation_t emTypeNation)
Purpose: 
Parameters: emTypeNation
Return: stOcrConfig*
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
stOcrConfig* ITS_InitOcr(emNation_t emTypeNation)
{
    stOcrConfig_t* stOcrConfig = new stOcrConfig_t;
    switch (emTypeNation)
    {
        case emNation_t::VietNam:
        {
            stOcrConfig->configOCR["width_det"]  = 128;
            stOcrConfig->configOCR["height_det"]  = 64;
            stOcrConfig->configOCR["width_recog"]  = 100;
            stOcrConfig->configOCR["height_recog"]  = 32;
            stOcrConfig->configOCR["det_db_thresh"]  = 0.3;
            stOcrConfig->configOCR["det_db_box_thresh"]  = 0.5;
            stOcrConfig->configOCR["det_db_unclip_ratio"]  = 1.6;
            stOcrConfig->configOCR["det_db_use_dilate"]  = 0;
            stOcrConfig->configOCR["det_use_polygon_score"]  = 1;
            stOcrConfig->configOCR["use_direction_classify"]  = 0;

            stOcrConfig->detector = loadModel(PATH_OCR_DET_VN);
            stOcrConfig->recognizer = loadModel(PATH_OCR_RECOG_VN);

            stOcrConfig->dictionary = ReadDict(PATH_DICTIONARY);
            stOcrConfig->dictionary.insert(stOcrConfig->dictionary.begin(), "#");
            stOcrConfig->dictionary.push_back(" ");

            std::cout << "[OCR] Configuration OCR VietNam finish." << std::endl;

            break;
        }
        case emNation_t::USA:
        {
            stOcrConfig->configOCR["width_det"]  = 128;
            stOcrConfig->configOCR["height_det"]  = 64;
            stOcrConfig->configOCR["width_recog"]  = 100;
            stOcrConfig->configOCR["height_recog"]  = 32;
            stOcrConfig->configOCR["det_db_thresh"]  = 0.3;
            stOcrConfig->configOCR["det_db_box_thresh"]  = 0.5;
            stOcrConfig->configOCR["det_db_unclip_ratio"]  = 1.6;
            stOcrConfig->configOCR["det_db_use_dilate"]  = 0;

            stOcrConfig->detector = loadModel(PATH_OCR_DET_US);
            stOcrConfig->recognizer = loadModel(PATH_OCR_RECOG_US);

            stOcrConfig->dictionary = ReadDict(PATH_DICTIONARY);
            stOcrConfig->dictionary.insert(stOcrConfig->dictionary.begin(), "#");
            stOcrConfig->dictionary.push_back(" ");

            std::cout << "[OCR] Configuration OCR USA finish." << std::endl;

            break;
        }
        case emNation_t::Malaysia:
        {
            stOcrConfig->configOCR["width_det"]  = 128;
            stOcrConfig->configOCR["height_det"]  = 64;
            stOcrConfig->configOCR["width_recog"]  = 100;
            stOcrConfig->configOCR["height_recog"]  = 32;
            stOcrConfig->configOCR["det_db_thresh"]  = 0.3;
            stOcrConfig->configOCR["det_db_box_thresh"]  = 0.5;
            stOcrConfig->configOCR["det_db_unclip_ratio"]  = 1.6;
            stOcrConfig->configOCR["det_db_use_dilate"]  = 0;
            stOcrConfig->configOCR["det_use_polygon_score"]  = 1;
            stOcrConfig->configOCR["use_direction_classify"]  = 0;

            stOcrConfig->detector = loadModel(PATH_OCR_DET_MALAY);
            stOcrConfig->recognizer = loadModel(PATH_OCR_RECOG_MALAY);

            stOcrConfig->dictionary = ReadDict(PATH_DICTIONARY);
            stOcrConfig->dictionary.insert(stOcrConfig->dictionary.begin(), "#");
            stOcrConfig->dictionary.push_back(" ");
            
            std::cout << "[OCR] Configuration OCR Malaysia finish." << std::endl;

            break;
        }
        default:
        {
            std::cerr << "OCR of Nation is unsupported." << std::endl;
            break;
        }
    }

    std::cout << "[ITS] Initialize OCR successfully." << std::endl;

    return stOcrConfig;
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
int ITS_PlateTracking(ObjectTracking* & pTracker, emNation_t emTypeNation, std::vector<ObjectTrace> stObjects, std::vector<TrackingTrace>& stTracks)
{
    switch (emTypeNation)
    {
        case emNation_t::VietNam:
        {
            pTracker->process_anpr_VN(stObjects, stTracks, 3.0, 10.0, 5.0, 7.0);
            break;
        }
        case emNation_t::USA:
        {
            pTracker->process_wh(stObjects, stTracks, 5.0, 5.0);
            break;
        }
        case emNation_t::Malaysia:
        {
            pTracker->process_wh(stObjects, stTracks, 5.0, 5.0);
            break;
        }
        default:
        {
            pTracker->process_wh(stObjects, stTracks, 5.0, 5.0);
            break;
        }
    }

    return STATUS_SUCCESS;
}

/*-------------------------------------------------------------------------------
Function: bool ITS_CheckPlate(stPlateInfor_t& stPlate, int iWidthImage, int iHeightImage)
Purpose: 
Parameters: stPlate, iWidthImage, iHeightImage
Return: bool
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
bool ITS_CheckPlate(stPlateInfor_t& stPlate, int iWidthImage, int iHeightImage)
{
    if (stPlate.riBoxPlate.area() < 1500) return false;

    if ((stPlate.riBoxPlate.tl().x > 5) && (stPlate.riBoxPlate.tl().y > 5) && (stPlate.riBoxPlate.br().x < (iWidthImage - 5)) 
        && (stPlate.riBoxPlate.br().y < (iHeightImage - 5)))
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*-------------------------------------------------------------------------------
Function: cv::Rect2f ITS_GetBox4k(cv::Rect2f& rfBoxFhd, int iCols4k, int iRows4k)
Purpose: 
Parameters: rfBoxFhd, iCols4k, iRows4k
Return: cv::Rect2f
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
cv::Rect2f ITS_GetBox4k(cv::Rect2f& rfBoxFhd, int iCols4k, int iRows4k)
{
    if(iCols4k == WIDTH_4K && iRows4k == HEIGHT_4K) {
        cv::Rect2f rfBox4k;
        rfBox4k.x = rfBoxFhd.x * SCALE_FHD_4K;
        rfBox4k.y = rfBoxFhd.y * SCALE_FHD_4K;
        rfBox4k.width = rfBoxFhd.width * SCALE_FHD_4K;
        rfBox4k.height = rfBoxFhd.height * SCALE_FHD_4K;
        return rfBox4k;
    } else {
        return rfBoxFhd;
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
std::vector<std::vector<std::vector<int>>> ITS_SortBoxes(std::vector<std::vector<std::vector<int>>> boxes)
{
    if (boxes.size() > 1)
    {
        std::vector<stRectText_t> stRectTexts;
        for(size_t i = 0; i < boxes.size(); i++)
        {
            stRectText_t stRectText;
            for (size_t j = 0; j < boxes[i].size(); j++)
            {
                cv::Point p = cv::Point(static_cast<int>(boxes[i][j][0]), static_cast<int>(boxes[i][j][1]));
                stRectText.points.push_back(p);
            }
            cv::Rect rect = cv::boundingRect(stRectText.points);
            stRectText.minPoint = rect.tl();
            stRectTexts.push_back(stRectText);
        }
        std::sort(stRectTexts.begin(), stRectTexts.end(), [](struct stRectText& a, struct stRectText& b) {return a.minPoint.x > b.minPoint.x;});
        std::vector<std::vector<std::vector<int>>> sortBoxes;
        for(size_t k = 0; k < stRectTexts.size(); k++)
        {
            std::vector<std::vector<int>> box;
            for(size_t l=0; l < stRectTexts[k].points.size(); l++)
            {
                std::vector<int> points;
                points.push_back(stRectTexts[k].points[l].x);
                points.push_back(stRectTexts[k].points[l].y);
                box.push_back(points);
            }
            sortBoxes.push_back(box);
        }
        return sortBoxes;
    }
    return boxes;
}

/*-------------------------------------------------------------------------------
Function: std::string ITS_CheckLicense(std::string& strLicense, emNation_t emTypeNation)
Purpose: 
Parameters: strLicense, emTypeNation
Return: std::string
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
std::string ITS_CheckLicense(std::string& strLicense, emNation_t emTypeNation)
{
    std::string strResult;
    switch (emTypeNation)
    {
        case emNation_t::VietNam:
        {
            std::regex regexLpFull("(((^[1-9][0-9][^0-9JW][0-9])|(^[^0-9JW][^0-9JW])|(^[1-9][0-9][^0-9JW]?[^0-9JW])|(^[1-9][0-9]MD[0-9]))[0-9]{4,5}$)|([0-9]{5}[A-Z][0-9])|([0-9]{5}[A-Z][A-Z][0-9]{2,3})");
            std::regex regexDetFail("^[1-9][0-9]{2}[^0-9JW]?[^0-9JW][0-9]{4,5}");
            std::regex regexRecheck("(^[1-9][0-9][^0-9JW][0-9][0-9]{1,2}$)|(^[^0-9JW][^0-9JW][0-9]{1,2}$)|(^[^0-9JW][^0-9JW][0-9]{3,4}$)|(^[1-9][0-9][^0-9JW]?[^0-9JW]$)|(^[1-9][0-9]MD[0-9][0-9]{1,2}$)");
            std::regex regex45("[0-9]{4,5}$");
            std::regex regex6("(^[^0-9JW][^0-9JW][0-9]{3,4}$)");
            std::regex regexMistake("(^[1-9][0-9][0-9][0-9])([0-9]{4,5}|[0-9]{3})$");
            std::regex regex2("(^[^0-9JW][^0-9JW])$");
            if(strLicense.length() >= 7 && strLicense.length() <= 10) {
                if(std::regex_match(strLicense, regexLpFull)) {
                    strResult = strLicense;
                } else if(std::regex_match(strLicense, regexDetFail)) {
                    if(std::regex_match(strLicense.substr(1, strLicense.length() - 1), regexLpFull)) {
                        strResult = strLicense.substr(1, strLicense.length() - 1);
                    } else {
                        strResult = "*" + strLicense.substr(2, strLicense.length() - 2);
                    }
                } else if(std::regex_match(strLicense, regexMistake)) {
                    strResult = strLicense.substr(0, 2) + "*" + strLicense.substr(3, strLicense.length() - 3);
                } else if(std::regex_match(strLicense.end() - 5, strLicense.end(), regex45)) {
                    strResult = "****" + strLicense.substr(strLicense.length() - 5, 5);
                } else if(std::regex_match(strLicense.end() - 4, strLicense.end(), regex45)) {
                    strResult = "****" + strLicense.substr(strLicense.length() - 4, 4);
                } else {
                    strResult = "Unknown";
                }
            } else if(strLicense.length() <= 6 && strLicense.length() >= 4) {
                if(std::regex_match(strLicense, regex6)) {
                    strResult = strLicense;
                } else if(std::regex_match(strLicense, regexRecheck)) {
                    if(std::regex_match(strLicense.substr(0, 2), regex2)) {
                        strResult = strLicense + "**";
                    } else {
                        strResult = strLicense.replace(strLicense.length(), 9 - strLicense.length(), 9 - strLicense.length(), '*');
                    }
                } else if(std::regex_match(strLicense.end() - 4, strLicense.end(), regex45)) {
                    strResult = "****" + strLicense.substr(strLicense.length() - 4, 4);
                } else {
                    strResult = "Unknown";
                }
            } else {
                strResult = "Unknown";
            }
            break;
        }
        case emNation_t::USA:
        {
            strResult = strLicense;
            break;
        }
        case emNation_t::Malaysia:
        {
            strResult = strLicense;
            break;
        }
        default:
        {
            strResult = "Unknown";
            break;
        }
    }
    return strResult;
}

/*-------------------------------------------------------------------------------
Function: std::string ITS_ReadText(cv::Mat& mCropPlate4k, emNation_t emTypeNation, std::string strTypePlate, float& fConfidence)
Purpose: 
Parameters: mCropPlate4k, emTypeNation, strTypePlate, fConfidence
Return: std::string
Comments: 
Modified: 
    <Modified by>
    <Date>
    <Change>
-------------------------------------------------------------------------------*/
std::string ITS_ReadText(stOcrConfig_t* & stOcrConfig, cv::Mat& mCropPlate4k, emNation_t emTypeNation, std::string strTypePlate, float& fConfidence)
{
    std::string strResult;
    if (mCropPlate4k.empty())
    {
        strResult = "N/A";
        fConfidence = 0;
    }
    else
    {
        std::vector<std::string> strRectTexts;
        std::vector<float> fRectTexts;
        switch (emTypeNation)
        {
            case emNation_t::VietNam:
            {
                if (strTypePlate == "VnRect")
                {
                    stOcrConfig->configOCR["width_det"] = 128;
                    stOcrConfig->configOCR["height_det"] = 64;
                    std::vector<std::vector<std::vector<int>>> boxes = RunDetModel(stOcrConfig->detector, mCropPlate4k, stOcrConfig->configOCR);
                    RunRecModel(boxes, mCropPlate4k, stOcrConfig->recognizer, strRectTexts, fRectTexts, stOcrConfig->dictionary, stOcrConfig->configOCR);
                }
                else if (strTypePlate == "VnSquare")
                {
                    stOcrConfig->configOCR["width_det"] = 128;
                    stOcrConfig->configOCR["height_det"] = 96;
                    std::vector<std::vector<std::vector<int>>> boxes = RunDetModel(stOcrConfig->detector, mCropPlate4k, stOcrConfig->configOCR);
                    RunRecModel(boxes, mCropPlate4k, stOcrConfig->recognizer, strRectTexts, fRectTexts, stOcrConfig->dictionary, stOcrConfig->configOCR);
                }
                break;
            }
            case emNation_t::USA:
            {
                std::vector<std::vector<std::vector<int>>> boxes = RunDetModel(stOcrConfig->detector, mCropPlate4k, stOcrConfig->configOCR);
                std::vector<std::vector<std::vector<int>>> sortBoxes = ITS_SortBoxes(boxes);
                RunRecModel(boxes, mCropPlate4k, stOcrConfig->recognizer, strRectTexts, fRectTexts, stOcrConfig->dictionary, stOcrConfig->configOCR);
                break;
            }
            case emNation_t::Malaysia:
            {
                std::vector<std::vector<std::vector<int>>> boxes = RunDetModel(stOcrConfig->detector, mCropPlate4k, stOcrConfig->configOCR);
                RunRecModel(boxes, mCropPlate4k, stOcrConfig->recognizer, strRectTexts, fRectTexts, stOcrConfig->dictionary, stOcrConfig->configOCR);
                break;
            }
            default:
                break;
        }

        float fScore = 0;
        if (strRectTexts.size() > 0)
        {
            for (size_t i = 0; i < strRectTexts.size(); i++)
            {
                if (!isnan(fRectTexts[i]))
                {
                    if (fRectTexts[i] < THRESHOLD_OCR)
                    {
                        strRectTexts[i] = "";
                    }
                    else
                    {
                        fScore += fRectTexts[i];
                    }
                }
                strResult += strRectTexts[i];
            }
            fConfidence = fScore / strRectTexts.size();

            strResult = ITS_CheckLicense(strResult, emTypeNation);
        }
        else
        {
            strResult = "Unknown";
            fScore = 0;
        }
    }

    return strResult;
}

// /*-------------------------------------------------------------------------------
// Function: template <typename _stTracks>
//           void ITS_DeleteLoseTrack(_stTracks& stTracks, std::vector<TrackingTrace>& stObjectTracks)
// Purpose: 
// Parameters: stTracks, stObjectTracks
// Return: void
// Comments: 
// Modified: 
//     <Modified by>
//     <Date>
//     <Change>
// -------------------------------------------------------------------------------*/
// template <typename _stTracks>
// void ITS_DeleteLoseTrack(_stTracks& stTracks, std::vector<TrackingTrace>& stObjectTracks)
// {
//     for (auto it = stTracks.begin(); it != stTracks.end(); )
//     {
//         const int iID = (*it).iTrackID;
//         const auto p = find_if(stObjectTracks.begin(), stObjectTracks.end(),
//                                 [iID] (const TrackingTrace& a) { return (a.m_ID == iID); });

//         if (p == stObjectTracks.end() && it != stTracks.end())
//         {
//             it = stTracks.erase(it);
//         }
//         else
//         {
//             it++;
//         }
//     }
// }
