#include "AnprRecognizer.hpp"

AnprRecognizer::AnprRecognizer(/* args */)
{
    this->detector = new AnprDetector();
    this->tracker = new ObjectTracking();
    this->vehicle_detector = new ObjectDetector();
    m_pVehicleTracker = new ObjectTracking();
    // this->vehicle_attributes = new ObjectDetector();
}

AnprRecognizer::~AnprRecognizer()
{
    if(this->ocrVN != nullptr)  delete this->ocrVN;

    if(this->ocrUS != nullptr)  delete this->ocrUS;

    if(this->ocrMalay != nullptr)  delete this->ocrMalay;

    if(this->ocrChina != nullptr)  delete this->ocrChina;

    if(this->detector != nullptr) delete this->detector;

    if(this->vehicle_detector != nullptr) delete this->vehicle_detector;

    if (m_pVehicleTracker != nullptr)
    {
        delete m_pVehicleTracker;
    }

    // if(this->vehicle_attributes != nullptr) delete this->vehicle_attributes;

    if(this->tracker != nullptr) delete this->tracker;
}

int AnprRecognizer::init(Nations nation)
{
    this->nations = nation;
    // init detector number plate ( VN, US, ...)
    if(!this->detector->init(nation))
        return STATUS_FAILED;

    // init ocr
    if(nation == Nations::VN)
    {
        std::cout << "OCR model: " << PATH_OCR_DET_VN << ", " << PATH_OCR_RECOG_VN << endl;
        if(initVn(PATH_OCR_DET_VN, PATH_OCR_RECOG_VN) != STATUS_SUCCESS)
            return STATUS_FAILED;
    }
    else if(nation == Nations::US)
    {
        std::cout << "OCR model: " << PATH_OCR_DET_US << ", " << PATH_OCR_RECOG_US << endl;
        if(initUS(PATH_OCR_DET_US, PATH_OCR_RECOG_US) != STATUS_SUCCESS)
            return STATUS_FAILED;
    }    
    else if(nation == Nations::MALAY)
    {
        this->listEvent.clear();
        std::cout << "OCR model: " << PATH_OCR_DET_MALAY << ", " << PATH_OCR_RECOG_MALAY << endl;
        if(initMalay(PATH_OCR_DET_MALAY, PATH_OCR_RECOG_MALAY) != STATUS_SUCCESS)
            return STATUS_FAILED;
    }
    else if (nation == Nations::China) // lt
    {
        std::cout << "OCR model: " << PATH_OCR_DET_CHINA << ", " << PATH_OCR_RECOG_CHINA << endl;
        if(initMalay(PATH_OCR_DET_CHINA, PATH_OCR_RECOG_CHINA) != STATUS_SUCCESS)
        {
            return STATUS_FAILED;
        }
    }

    return STATUS_SUCCESS;
}

Nations vehicle_nations;

int AnprRecognizer::initVehicle(settingVehicle settings)
{
    this->nations = settings.nation;
    vehicle_nations = settings.nation; // threadVehicle
    this->objectsTrack = settings.objectsTrack;
    // init detector number plate ( VN, US, ...)
    if(!this->detector->init(this->nations))
        return STATUS_FAILED;

    // if(!this->vehicle_detector->initObjectDetector("snpe", "traffic", "DSP"))
    //     return STATUS_FAILED;
    this->vehicle_detector->initObjectDetector("snpe", "traffic", "DSP");

    // this->vehicle_attributes->initObjectAttributes("tflite", "attributes_weight_quant");

    // init ocr
    if(this->nations == Nations::VN)
    {
        std::cout << "OCR model: " << PATH_OCR_DET_VN << ", " << PATH_OCR_RECOG_VN << endl;
        if(initVn(PATH_OCR_DET_VN, PATH_OCR_RECOG_VN) != STATUS_SUCCESS)
            return STATUS_FAILED;
    }
    else if(this->nations == Nations::US)
    {
        std::cout << "OCR model: " << PATH_OCR_DET_US << ", " << PATH_OCR_RECOG_US << endl;
        if(initUS(PATH_OCR_DET_US, PATH_OCR_RECOG_US) != STATUS_SUCCESS)
            return STATUS_FAILED;
    }    
    else if(this->nations == Nations::MALAY)
    {
        std::cout << "OCR model: " << PATH_OCR_DET_MALAY << ", " << PATH_OCR_RECOG_MALAY << endl;
        if(initMalay(PATH_OCR_DET_MALAY, PATH_OCR_RECOG_MALAY) != STATUS_SUCCESS)
            return STATUS_FAILED;
    } 
    return STATUS_SUCCESS;
}

int AnprRecognizer::initVehicle_lt(settingVehicle stSetVehicle)
{
    this->nations = stSetVehicle.nation;
    vehicle_nations = stSetVehicle.nation; // threadVehicle
    this->objectsTrack = stSetVehicle.objectsTrack;
    // init detector number plate ( VN, US, ...)
    if(!this->detector->init(this->nations))
        return STATUS_FAILED;

    // if(!this->vehicle_detector->initObjectDetector("snpe", "traffic", "DSP"))
    //     return STATUS_FAILED;
    this->vehicle_detector->initObjectDetector("snpe", "traffic", "DSP");

    // this->vehicle_attributes->initObjectAttributes("tflite", "attributes_weight_quant");

    // init ocr
    if(this->nations == Nations::VN)
    {
        std::cout << "OCR model: " << PATH_OCR_DET_VN << ", " << PATH_OCR_RECOG_VN << endl;
        if(initVn(PATH_OCR_DET_VN, PATH_OCR_RECOG_VN) != STATUS_SUCCESS)
            return STATUS_FAILED;
    }
    else if(this->nations == Nations::US)   
    {
        std::cout << "OCR model: " << PATH_OCR_DET_US << ", " << PATH_OCR_RECOG_US << endl;
        if(initUS(PATH_OCR_DET_US, PATH_OCR_RECOG_US) != STATUS_SUCCESS)
            return STATUS_FAILED;
    }    
    else if(this->nations == Nations::MALAY)
    {
        std::cout << "OCR model: " << PATH_OCR_DET_MALAY << ", " << PATH_OCR_RECOG_MALAY << endl;
        if(initMalay(PATH_OCR_DET_MALAY, PATH_OCR_RECOG_MALAY) != STATUS_SUCCESS)
            return STATUS_FAILED;
    }

    return STATUS_SUCCESS;
}

int AnprRecognizer::initVn(std::string pathDet, std::string pathRecog)
{
    this->ocrVN = new OcrConfig;
    this->ocrVN->configOCR["width_det"]  = 192;
    this->ocrVN->configOCR["height_det"]  = 64;
    this->ocrVN->configOCR["width_recog"]  = 100;
    this->ocrVN->configOCR["height_recog"]  = 32;
    this->ocrVN->configOCR["det_db_thresh"]  = 0.3;
    this->ocrVN->configOCR["det_db_box_thresh"]  = 0.5;
    this->ocrVN->configOCR["det_db_unclip_ratio"]  = 1.6;
    this->ocrVN->configOCR["det_db_use_dilate"]  = 0;
    this->ocrVN->configOCR["det_use_polygon_score"]  = 1;
    this->ocrVN->configOCR["use_direction_classify"]  = 0;

    this->ocrVN->detector = loadModel(pathDet);
    this->ocrVN->recog = loadModel(pathRecog);

    this->ocrVN->dict = ReadDict(PATH_DICTIONARY);
    this->ocrVN->dict.insert(this->ocrVN->dict.begin(), "#"); // blank char for ctc
    this->ocrVN->dict.push_back(" ");
    printf("[INFO] - Init OCR Vietnam successfully\n");
    return STATUS_SUCCESS;
}

int AnprRecognizer::initUS(std::string pathDet, std::string pathRecog)
{
    this->ocrUS = new OcrConfig;

    this->ocrUS->configOCR["width_det"]  =128;
    this->ocrUS->configOCR["height_det"]  = 96;
    this->ocrUS->configOCR["width_recog"]  = 100;
    this->ocrUS->configOCR["height_recog"]  = 32;
    this->ocrUS->configOCR["det_db_thresh"]  = 0.3;
    this->ocrUS->configOCR["det_db_box_thresh"]  = 0.5;
    this->ocrUS->configOCR["det_db_unclip_ratio"]  = 1.6;
    this->ocrUS->configOCR["use_direction_classify"]  = 0;
    
    this->ocrUS->detector = loadModel(pathDet);
    this->ocrUS->recog = loadModel(pathRecog);

    this->ocrUS->dict = ReadDict(PATH_DICTIONARY);
    this->ocrUS->dict.insert(this->ocrUS->dict.begin(), "#"); // blank char for ctc
    this->ocrUS->dict.push_back(" ");
    printf("[INFO] - Init model text recognition in US successfully\n");
    return STATUS_SUCCESS;
}

int AnprRecognizer::initMalay(std::string pathDet, std::string pathRecog)
{
    this->ocrMalay = new OcrConfig;

    this->ocrMalay->configOCR["width_det"]  = 128;
    this->ocrMalay->configOCR["height_det"]  = 96;
    this->ocrMalay->configOCR["width_recog"]  = 100;
    this->ocrMalay->configOCR["height_recog"]  = 32;
    this->ocrMalay->configOCR["det_db_thresh"]  = 0.3;
    this->ocrMalay->configOCR["det_db_box_thresh"]  = 0.5;
    this->ocrMalay->configOCR["det_db_unclip_ratio"]  = 1.6;
    this->ocrMalay->configOCR["use_direction_classify"]  = 0;
    
    this->ocrMalay->detector = loadModel(pathDet);
    this->ocrMalay->recog = loadModel(pathRecog);
    this->ocrMalay->dict = ReadDict(PATH_DICTIONARY);
    this->ocrMalay->dict.insert(this->ocrMalay->dict.begin(), "#"); // blank char for ctc
    this->ocrMalay->dict.push_back(" ");
    printf("[INFO] - Init model text recognition in Malaysia successfully\n");
    return STATUS_SUCCESS;
}

int AnprRecognizer::initChina(std::string pathDet, std::string pathRecog)
{
    this->ocrUS = new OcrConfig;

    this->ocrUS->configOCR["width_det"]  =128;
    this->ocrUS->configOCR["height_det"]  = 96;
    this->ocrUS->configOCR["width_recog"]  = 100;
    this->ocrUS->configOCR["height_recog"]  = 32;
    this->ocrUS->configOCR["det_db_thresh"]  = 0.3;
    this->ocrUS->configOCR["det_db_box_thresh"]  = 0.5;
    this->ocrUS->configOCR["det_db_unclip_ratio"]  = 1.6;
    this->ocrUS->configOCR["use_direction_classify"]  = 0;
    
    this->ocrUS->detector = loadModel(pathDet);
    this->ocrUS->recog = loadModel(pathRecog);

    this->ocrUS->dict = ReadDict(PATH_DICTIONARY_CHINA);
    this->ocrUS->dict.insert(this->ocrUS->dict.begin(), "#"); // blank char for ctc
    this->ocrUS->dict.push_back(" ");
    printf("[INFO] - Init model text recognition in China successfully\n");
    return STATUS_SUCCESS;
}

std::string AnprRecognizer::readText( cv::Mat& img, Nations nation, float& confidence)
{
    std::string textOut = "";
    if(!img.empty())
    {
        std::vector<std::string> rec_text;
        std::vector<float> rec_text_score;
        if(nation == Nations::VnSquare)
        {
            this->ocrVN->configOCR["width_det"]  = 128;
            this->ocrVN->configOCR["height_det"]  = 96;
            std::vector<std::vector<std::vector<int>>> boxes = RunDetModel(this->ocrVN->detector, img, this->ocrVN->configOCR); 
            RunRecModel(boxes, img, this->ocrVN->recog, rec_text, rec_text_score, this->ocrVN->dict, this->ocrVN->configOCR);
        }
        else if(nation == Nations::VnRect)
        {
            this->ocrVN->configOCR["width_det"]  = 128;
            this->ocrVN->configOCR["height_det"]  = 64;
            std::vector<std::vector<std::vector<int>>> boxes = RunDetModel(this->ocrVN->detector, img, this->ocrVN->configOCR);
            RunRecModel(boxes, img, this->ocrVN->recog, rec_text, rec_text_score, this->ocrVN->dict, this->ocrVN->configOCR);
        }
        else if(nation == Nations::US)
        {
            std::vector<std::vector<std::vector<int>>> boxes = RunDetModel(this->ocrUS->detector, img, this->ocrUS->configOCR);
            std::vector<std::vector<std::vector<int>>> sort_boxes = sortBoxes(boxes);
            RunRecModel(sort_boxes, img, this->ocrUS->recog, rec_text, rec_text_score, this->ocrUS->dict, this->ocrUS->configOCR);
            // Visualization(img, boxes);
        }
        else if(nation == Nations::MALAY)
        {
            std::vector<std::vector<std::vector<int>>> boxes = RunDetModel(this->ocrMalay->detector, img, this->ocrMalay->configOCR);
            std::vector<std::vector<std::vector<int>>> sort_boxes = sortBoxes(boxes);
            RunRecModel(sort_boxes, img, this->ocrMalay->recog, rec_text, rec_text_score, this->ocrMalay->dict, this->ocrMalay->configOCR);
        }
        else if(nation == Nations::China)
        {
            std::vector<std::vector<std::vector<int>>> boxes = RunDetModel(this->ocrMalay->detector, img, this->ocrMalay->configOCR);
            // std::vector<std::vector<std::vector<int>>> sort_boxes = sortBoxes(boxes);
            RunRecModel(boxes, img, this->ocrMalay->recog, rec_text, rec_text_score, this->ocrMalay->dict, this->ocrMalay->configOCR);
        }

        float score = 0;
        if(rec_text.size() > 0)
        { 
            for (int i = 0; i < rec_text.size(); i++)
            {                       
                if(!isnan(rec_text_score[i]))
                {
                    if(rec_text_score[i] < THRESHOLD_OCR) {
                        rec_text[i] = "";
                        // return "Unknown";
                    } else {
                        score += rec_text_score[i];
                    }
                }
                textOut += rec_text[i];                       
            }          
            confidence = score / rec_text.size();
            if(this->nations == Nations::VN)
            {   
                textOut = checkLicenseVN(textOut);
            }
        }
        else 
        {
            textOut = "Unknown";
            confidence = 0;
        }
    }
    
    return textOut;
}

std::vector<std::vector<std::vector<int>>> AnprRecognizer::sortBoxes(std::vector<std::vector<std::vector<int>>> boxes)
{
    if(boxes.size() > 1) {
        std::vector<rectText> rectTexts;
        for(int i=0; i<boxes.size(); i++) {
            rectText rectText;
            for(int j=0; j<boxes[i].size(); j++) {
                cv::Point p = cv::Point(static_cast<int>(boxes[i][j][0]), static_cast<int>(boxes[i][j][1]));
                rectText.points.push_back(p);
            }
            cv::Rect rect = cv::boundingRect(rectText.points);
            rectText.minPoint = rect.tl();
            rectTexts.push_back(rectText);
        }
        std::sort(rectTexts.begin(), rectTexts.end(), [](struct rectText &a, struct rectText &b) {return a.minPoint.x > b.minPoint.x;});
        std::vector<std::vector<std::vector<int>>> sortBoxes;
        for(int k=0; k<rectTexts.size(); k++) {
            std::vector<std::vector<int>> box;
            for(int n=0; n<rectTexts[k].points.size(); n++) {
                std::vector<int> points;
                points.push_back(rectTexts[k].points[n].x);
                points.push_back(rectTexts[k].points[n].y);
                box.push_back(points);
            }
            sortBoxes.push_back(box);
        }
        return sortBoxes;
    }
    return boxes;
}

std::string AnprRecognizer::checkLicenseVN(std::string& text)
{
    std::regex regexLpFull("(((^[1-9][0-9][^0-9JW][0-9])|(^[^0-9JW][^0-9JW])|(^[1-9][0-9][^0-9JW]?[^0-9JW])|(^[1-9][0-9]MD[0-9]))[0-9]{4,5}$)|([0-9]{5}[A-Z][0-9])|([0-9]{5}[A-Z][A-Z][0-9]{2,3})");
    std::regex regexDetFail("^[1-9][0-9]{2}[^0-9JW]?[^0-9JW][0-9]{4,5}");
    std::regex regexRecheck("(^[1-9][0-9][^0-9JW][0-9][0-9]{1,2}$)|(^[^0-9JW][^0-9JW][0-9]{1,2}$)|(^[^0-9JW][^0-9JW][0-9]{3,4}$)|(^[1-9][0-9][^0-9JW]?[^0-9JW]$)|(^[1-9][0-9]MD[0-9][0-9]{1,2}$)");
    std::regex regex45("[0-9]{4,5}$");
    std::regex regex6("(^[^0-9JW][^0-9JW][0-9]{3,4}$)");
    std::regex regexMistake("(^[1-9][0-9][0-9][0-9])([0-9]{4,5}|[0-9]{3})$");
    std::regex regex2("(^[^0-9JW][^0-9JW])$");
    if(text.length() >= 7 && text.length() <= 10) {
        if(std::regex_match(text, regexLpFull)) {
            return text;
        } else if(std::regex_match(text, regexDetFail)) {
            // return text.substr(1, text.length() - 1);
            if(std::regex_match(text.substr(1, text.length() - 1), regexLpFull)) {
                return text.substr(1, text.length() - 1);
            } else {
                return "*" + text.substr(2, text.length() - 2);
            }
        } else if(std::regex_match(text, regexMistake)) {
            return text.substr(0, 2) + "*" + text.substr(3, text.length() - 3);
        } else if(std::regex_match(text.end() - 5, text.end(), regex45)) {
            // return text.replace(0, text.length() - 5, text.length() - 5, '*');
            return "****" + text.substr(text.length() - 5, 5);
        } else if(std::regex_match(text.end() - 4, text.end(), regex45)) {
            // return text.replace(0, text.length() - 4, text.length() - 4, '*');
            return "****" + text.substr(text.length() - 4, 4);
        } else {
            return "Unknown";
        }
    } else if(text.length() <= 6 && text.length() >= 4) {
        if(std::regex_match(text, regex6)) {
            return text;
        } else if(std::regex_match(text, regexRecheck)) {
            if(std::regex_match(text.substr(0, 2), regex2)) {
                return text + "**";
            } else {
                return text.replace(text.length(), 9 - text.length(), 9 - text.length(), '*');
            }
            // return text.replace(text.length(), 9 - text.length(), 9 - text.length(), '*');
        } else if(std::regex_match(text.end() - 4, text.end(), regex45)) {
            // return text.insert(0, 9 - text.length(), '*');
            return "****" + text.substr(text.length() - 4, 4);
        } else {
            return "Unknown";
        }
    } else {
        return "Unknown";
    }
}

cv::Rect2f AnprRecognizer::getRect4K(cv::Rect2f rect_fhd, int cols_4K, int rows_4K)
{
    if(cols_4K == WIDTH_4K && rows_4K == HEIGHT_4K) {
        cv::Rect2f rect_4K;
        rect_4K.x = rect_fhd.x * SCALE_FHD_4K;
        rect_4K.y = rect_fhd.y * SCALE_FHD_4K;
        rect_4K.width = rect_fhd.width * SCALE_FHD_4K;
        rect_4K.height = rect_fhd.height * SCALE_FHD_4K;
        return rect_4K;
    } else {
        return rect_fhd;
    }
}

int AnprRecognizer::recognize(cv::Mat& img, cv::Mat &img_4K, std::vector<PlateInfor>& plates)
{
    if(!img.empty() && !img_4K.empty())
    {
        int cols_4K = img_4K.cols;
        int rows_4K = img_4K.rows;
        try
        {
            std::vector<ObjectTrace> objPlates;
            this->detector->detect(img, objPlates);

            if(objPlates.size() == 0) {
                this->listEvent.clear();
                return STATUS_SUCCESS;
            } else {
                if(this->listEvent.size() > 50) {
                    this->listEvent.erase(this->listEvent.begin(), this->listEvent.begin() + 24);
                }
            }

            plates.clear();
        
            for(int i = 0; i < objPlates.size(); i++)
            {
                cv::Rect2f rect_4K = getRect4K(objPlates[i].rect, cols_4K, rows_4K);
                cv::Mat imgPlate4K = img_4K(rect_4K).clone();

                cv::Mat imgPlate = img(objPlates[i].rect).clone();
                float confidence = 0;
                PlateInfor plate;
                if(isValidPlate(imgPlate))
                {
                    if(objPlates[i].label == "VnRect")
                    {
                        plate.license = readText(imgPlate4K, Nations::VnRect, confidence);
                    }          
                    else if(objPlates[i].label == "VnSquare")
                    {
                        plate.license = readText(imgPlate4K, Nations::VnSquare, confidence);
                    }           
                    if(objPlates[i].label == "US")
                    {
                        plate.license = readText(imgPlate4K, Nations::US, confidence); 
                    }
                    else if(objPlates[i].label == "Malay")
                    {
                        plate.license = readText(imgPlate4K, Nations::MALAY, confidence); 
                    }
                    if(!isnan(confidence) && !isnan(objPlates[i].score))
                        plate.score = (confidence + objPlates[i].score) / 2.0;
                    else plate.score = 0;
                }
                else
                {
                    plate.license = "Unknown";
                    plate.score = 0;
                }

                if(this->listEvent.size() == 0) {
                    plate.imgPlate = imgPlate;
                    plate.isNewEvent = true;
                    plate.rect = objPlates[i].rect;
                    plate.typePlate = objPlates[i].label;
                    plates.push_back(plate);

                    this->listEvent.push_back(plate);
                } else {
                    float x_center = objPlates[i].rect.x + objPlates[i].rect.width / 2;
                    float y_center = objPlates[i].rect.y + objPlates[i].rect.height / 2;
                    cv::Point2f point_center = cv::Point2f(x_center, y_center);
                    
                    int count_contains = 0;
                    for(int j=0; j<this->listEvent.size(); j++) {
                        if(this->listEvent[j].rect.contains(point_center) && (plate.license == this->listEvent[j].license)) {
                            this->listEvent[j].rect = objPlates[i].rect;
                            count_contains += 1;
                        }
                    }

                    if(count_contains != 1) {
                        plate.imgPlate = imgPlate;
                        plate.isNewEvent = true;
                        plate.rect = objPlates[i].rect;
                        plate.typePlate = objPlates[i].label;
                        plates.push_back(plate);

                        this->listEvent.push_back(plate);
                    }
                }
            }           
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    else
    {
        LOG_FAIL("Execute Anpr recognizer failed, please check your input");
        return STATUS_FAILED; 
    }
    return STATUS_SUCCESS;
}

int AnprRecognizer::trackAnpr(cv::Mat &img, cv::Mat &img_4K, std::vector<PlateInfor>& plates)
{
    plates.clear();
    if(!img.empty() && !img_4K.empty())
    {
        int cols_4K = img_4K.cols;
        int rows_4K = img_4K.rows;
        try
        {   
            // detect
            std::vector<ObjectTrace> objPlates;
            this->detector->detect(img, objPlates);

            // process tracking
            std::vector<TrackingTrace> tracks;

            if(this->nations == Nations::VN) {
                this->tracker->process_anpr_VN(objPlates, tracks, 3.0, 10.0, 5.0, 7.0);
            } else {
                this->tracker->process_wh(objPlates, tracks, 5.0, 5.0);
            }

            //delete object which is abandoned
            for(auto it = this->listPlateTracks.begin(); it != this->listPlateTracks.end();)
            {
                const int theId =  (*it).track_id;
                const auto p = find_if(tracks.begin(), tracks.end(), 
                                            [theId] ( const TrackingTrace& a ) { return (a.m_ID == theId);}); 
                if (p == tracks.end() && it != this->listPlateTracks.end())
                    it = this->listPlateTracks.erase(it);
                else 
                    it++;
            }

            // process new track and update old track
            for(auto track : tracks)
            {
                if(!track.isOutOfFrame)
                {
                    const int theId =  track.m_ID;
                    const auto p = find_if(this->listPlateTracks.begin(), this->listPlateTracks.end(), 
                                            [theId] ( const PlateInfor& a ) { return (a.track_id == theId);});  
                    
                    if (p != this->listPlateTracks.end()) // if find plate in list tracked plate
                    {
                        int dist = distance(this->listPlateTracks.begin(), p);
                        this->listPlateTracks[dist].rect = track.m_rect;
                        this->listPlateTracks[dist].isOutOfFrame = false;
                        if((this->listPlateTracks[dist].license == "Unknown" || this->listPlateTracks[dist].isDefectPlate) 
                            && this->listPlateTracks[dist].countUnknown <= MAX_COUNT_UNKNOWN)
                        {
                            // this->listPlateTracks[dist].isNewEvent = true;
                            cv::Rect2f rect_4K = getRect4K(track.m_rect, cols_4K, rows_4K);
                            cv::Mat imgPlate4K = img_4K(rect_4K).clone();
                            
                            cv::Mat imgPlate = img(this->listPlateTracks[dist].rect).clone();
                            float confidence = 0;
                            this->listPlateTracks[dist].imgPlate = imgPlate;
                            if(checkPlate(this->listPlateTracks[dist], img.cols, img.rows))
                            {
                                this->listPlateTracks[dist].isNewEvent = true;
                                if(track.m_type == "VnRect")
                                {
                                    this->listPlateTracks[dist].license = readText(imgPlate4K, Nations::VnRect, confidence);
                                }          
                                else if(track.m_type == "VnSquare")
                                {
                                    this->listPlateTracks[dist].license = readText(imgPlate4K, Nations::VnSquare, confidence);
                                }           
                                else if(track.m_type == "US")
                                {
                                    this->listPlateTracks[dist].license = readText(imgPlate4K, Nations::US, confidence); 
                                } 
                                else if(track.m_type == "Malay")
                                {
                                    this->listPlateTracks[dist].license = readText(imgPlate4K, Nations::MALAY, confidence); 
                                } 

                                if(!isnan(confidence))
                                    this->listPlateTracks[dist].score = confidence;
                                else this->listPlateTracks[dist].score = 0;

                                if(this->listPlateTracks[dist].license == "Unknown") {
                                    this->listPlateTracks[dist].countUnknown++;
                                    this->listPlateTracks[dist].isNewEvent = false;
                                } else if(this->listPlateTracks[dist].license.size() < 7 || this->listPlateTracks[dist].license.size() > 10) {
                                    this->listPlateTracks[dist].countUnknown++;
                                    this->listPlateTracks[dist].isNewEvent = false;
                                } else {
                                    this->listPlateTracks[dist].isDefectPlate = false;
                                }
                            }
                            else
                            {
                                this->listPlateTracks[dist].license = "Unknown";
                                this->listPlateTracks[dist].score = 0;
                                // this->listPlateTracks[dist].countUnknown++;
                                this->listPlateTracks[dist].isNewEvent = false;
                            }                            
                        } 
                        else if(this->listPlateTracks[dist].countUnknown == (MAX_COUNT_UNKNOWN + 1)) {
                            this->listPlateTracks[dist].countUnknown++;
                            this->listPlateTracks[dist].isDefectPlate = false;
                            this->listPlateTracks[dist].isNewEvent = true;
                        } 
                        else {
                            this->listPlateTracks[dist].isNewEvent = false;
                        } 
                        
                    }
                    else  // if find new plate
                    {
                        cv::Rect2f rect_4K = getRect4K(track.m_rect, cols_4K, rows_4K);
                        cv::Mat imgPlate4K = img_4K(rect_4K).clone();

                        cv::Mat imgPlate = img(track.m_rect).clone();
                        float confidence = 0;
                        PlateInfor plate;
                        plate.imgPlate = imgPlate;
                        // plate.isNewEvent = true;
                        plate.track_id = track.m_ID;
                        plate.rect = track.m_rect;
                        plate.typePlate = track.m_type;   
                        plate.isOutOfFrame = false;
                        if(checkPlate(plate, img.cols, img.rows))
                        {
                            plate.isNewEvent = true;
                            if(track.m_type == "VnRect")
                            {
                                plate.license = readText(imgPlate4K, Nations::VnRect, confidence);
                            }          
                            else if(track.m_type == "VnSquare")
                            {
                                plate.license = readText(imgPlate4K, Nations::VnSquare, confidence);
                            }           
                            else if(track.m_type == "US")
                            {
                                plate.license = readText(imgPlate4K, Nations::US, confidence); 
                            } 
                            else if(track.m_type == "Malay")
                            {
                                plate.license = readText(imgPlate4K, Nations::MALAY, confidence); 
                            }

                            if(!isnan(confidence))
                                plate.score = confidence;
                            else {
                                plate.score = 0;
                                // plate.license = "Unknown";
                            }

                            if(plate.license.size() < 7 || plate.license.size() > 10) {
                                plate.isDefectPlate = true;
                                plate.countUnknown = 1;
                            }
                        } else {
                            plate.license = "Unknown";
                            plate.score = 0;
                            // plate.countUnknown = 1;
                        } 

                        if(plate.license == "Unknown" || plate.isDefectPlate) {
                            plate.isNewEvent = false;
                        }

                        this->listPlateTracks.push_back(plate);
                    }
                }
                else
                {
                    const int theId =  track.m_ID;
                    const auto p = find_if(this->listPlateTracks.begin(), this->listPlateTracks.end(), 
                                            [theId] ( const PlateInfor& a ) { return (a.track_id == theId);});                         
                    if (p != this->listPlateTracks.end()) 
                    {
                        int dist = distance(this->listPlateTracks.begin(), p);
                        this->listPlateTracks[dist].isOutOfFrame = true;
                        
                    }
                } 
            }  
            //get list of output plates
            for(auto p : this->listPlateTracks)
            {
                if(p.isOutOfFrame != true) plates.push_back(p);
            }                  
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    else
    {
        LOG_FAIL("Execute Anpr recognizer failed, please check your input");
        return STATUS_FAILED; 
    }
    return STATUS_SUCCESS;
}

int AnprRecognizer::trackAnpr_cn(cv::Mat &img, cv::Mat &img_4K, std::vector<PlateInfor>& plates)
{
    plates.clear();
    if(!img.empty() && !img_4K.empty())
    {
        int cols_4K = img_4K.cols;
        int rows_4K = img_4K.rows;
        try
        {   
            // detect
            std::vector<ObjectTrace> objPlates;
            this->detector->detect(img, objPlates);

            // process tracking
            std::vector<TrackingTrace> tracks;

            this->tracker->process_wh(objPlates, tracks, 5.0, 5.0);

            //delete object which is abandoned
            for(auto it = this->listPlateTracks.begin(); it != this->listPlateTracks.end();)
            {
                const int theId =  (*it).track_id;
                const auto p = find_if(tracks.begin(), tracks.end(), 
                                            [theId] ( const TrackingTrace& a ) { return (a.m_ID == theId);}); 
                if (p == tracks.end() && it != this->listPlateTracks.end())
                    it = this->listPlateTracks.erase(it);
                else 
                    it++;
            }

            // process new track and update old track
            for(auto track : tracks)
            {
                if(!track.isOutOfFrame)
                {
                    const int theId =  track.m_ID;
                    const auto p = find_if(this->listPlateTracks.begin(), this->listPlateTracks.end(), 
                                            [theId] ( const PlateInfor& a ) { return (a.track_id == theId);});  
                    
                    if (p != this->listPlateTracks.end()) // if find plate in list tracked plate
                    {
                        int dist = distance(this->listPlateTracks.begin(), p);
                        this->listPlateTracks[dist].rect = track.m_rect;
                        this->listPlateTracks[dist].isOutOfFrame = false;
                        if((this->listPlateTracks[dist].license == "Unknown") && this->listPlateTracks[dist].countUnknown <= MAX_COUNT_UNKNOWN)
                        {
                            // this->listPlateTracks[dist].isNewEvent = true;
                            cv::Rect2f rect_4K = getRect4K(track.m_rect, cols_4K, rows_4K);
                            cv::Mat imgPlate4K = img_4K(rect_4K).clone();
                            
                            cv::Mat imgPlate = img(this->listPlateTracks[dist].rect).clone();
                            float confidence = 0;
                            this->listPlateTracks[dist].imgPlate = imgPlate;
                            if(checkPlate(this->listPlateTracks[dist], img.cols, img.rows))
                            {
                                this->listPlateTracks[dist].isNewEvent = true;
                                if(track.m_type == "china")
                                {
                                    this->listPlateTracks[dist].license = readText(imgPlate4K, Nations::China, confidence);
                                }

                                if(this->listPlateTracks[dist].license == "Unknown") {
                                    this->listPlateTracks[dist].countUnknown++;
                                    this->listPlateTracks[dist].isNewEvent = false;
                                }
                            }
                            else
                            {
                                this->listPlateTracks[dist].license = "Unknown";
                                // this->listPlateTracks[dist].countUnknown++;
                                this->listPlateTracks[dist].isNewEvent = false;
                            }
                        } 
                        else if(this->listPlateTracks[dist].countUnknown == (MAX_COUNT_UNKNOWN + 1)) {
                            this->listPlateTracks[dist].countUnknown++;
                            this->listPlateTracks[dist].isNewEvent = true;
                        }
                        else {
                            this->listPlateTracks[dist].isNewEvent = false;
                        }
                        
                    }
                    else  // if find new plate
                    {
                        cv::Rect2f rect_4K = getRect4K(track.m_rect, cols_4K, rows_4K);
                        cv::Mat imgPlate4K = img_4K(rect_4K).clone();

                        cv::Mat imgPlate = img(track.m_rect).clone();
                        float confidence = 0;
                        PlateInfor plate;
                        plate.imgPlate = imgPlate;
                        // plate.isNewEvent = true;
                        plate.track_id = track.m_ID;
                        plate.rect = track.m_rect;
                        plate.typePlate = track.m_type;   
                        plate.isOutOfFrame = false;
                        if(checkPlate(plate, img.cols, img.rows))
                        {
                            plate.isNewEvent = true;
                            if(track.m_type == "china")
                            {
                                plate.license = readText(imgPlate4K, Nations::China, confidence);
                            }
                        }
                        else
                        {
                            plate.license = "Unknown";
                            // plate.countUnknown = 1;
                        }

                        if(plate.license == "Unknown")
                        {
                            plate.isNewEvent = false;
                        }

                        this->listPlateTracks.push_back(plate);
                    }
                }
                else
                {
                    const int theId =  track.m_ID;
                    const auto p = find_if(this->listPlateTracks.begin(), this->listPlateTracks.end(), 
                                            [theId] ( const PlateInfor& a ) { return (a.track_id == theId);});                         
                    if (p != this->listPlateTracks.end()) 
                    {
                        int dist = distance(this->listPlateTracks.begin(), p);
                        this->listPlateTracks[dist].isOutOfFrame = true;
                        
                    }
                } 
            }  
            //get list of output plates
            for(auto p : this->listPlateTracks)
            {
                if(p.isOutOfFrame != true) plates.push_back(p);
            }                  
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    else
    {
        LOG_FAIL("Execute Anpr recognizer failed, please check your input");
        return STATUS_FAILED; 
    }
    return STATUS_SUCCESS;
}

// int AnprRecognizer::trackAnprVerify(Mat &img, std::vector<PlateInfor>& plates)
// {
//     plates.clear();
//     if(!img.empty())
//     {
//         try
//         {   
//             // detect
//             std::vector<ObjectTrace> objPlates;
//             this->detector->detect(img, objPlates);

//             // process tracking
//             std::vector<TrackingTrace> tracks;
//             if(this->nations == Nations::VN) {
//                 this->tracker->process_anpr_VN(objPlates, tracks, 3.0, 10.0, 5.0, 7.0);
//             } else {
//                 this->tracker->process_wh(objPlates, tracks, 5.0, 5.0);
//             }

//             //delete object which is abandoned
//             for(auto it = this->listPlateTracks.begin(); it != this->listPlateTracks.end();)
//             {
//                 const int theId =  (*it).track_id;
//                 const auto p = find_if(tracks.begin(), tracks.end(), 
//                                             [theId] ( const TrackingTrace& a ) { return (a.m_ID == theId);}); 
//                 if (p == tracks.end() && it != this->listPlateTracks.end())
//                     it = this->listPlateTracks.erase(it);                
//                 else 
//                     it++;
//             }

//             // process new track and update old track
//             for(auto track : tracks)
//             {
//                 if(!track.isOutOfFrame)
//                 {
//                     const int theId =  track.m_ID;
//                     const auto p = find_if(this->listPlateTracks.begin(), this->listPlateTracks.end(), 
//                                             [theId] ( const PlateInfor& a ) { return (a.track_id == theId);});  
                    
//                     if (p != this->listPlateTracks.end()) // if find plate in list tracked plate
//                     {
//                         int dist = distance(this->listPlateTracks.begin(), p);
//                         this->listPlateTracks[dist].rect = track.m_rect;
//                         this->listPlateTracks[dist].isOutOfFrame = false;
//                         if(((this->listPlateTracks[dist].license == "Unknown" || this->listPlateTracks[dist].isDefectPlate) 
//                             && this->listPlateTracks[dist].countUnknown <= MAX_COUNT_UNKNOWN)
//                             || this->listPlateTracks[dist].countVerify <= MAX_COUNT_VERIFY)
//                         {
//                             // this->listPlateTracks[dist].isNewEvent = true;
//                             cv::Mat imgPlate = img(this->listPlateTracks[dist].rect).clone();
//                             float confidence = 0;
//                             this->listPlateTracks[dist].imgPlate = imgPlate;
//                             if(checkPlate(this->listPlateTracks[dist], img.cols, img.rows))
//                             {
//                                 this->listPlateTracks[dist].isNewEvent = true;
//                                 if(track.m_type == "VnRect")
//                                 {
//                                     this->listPlateTracks[dist].license = readText(imgPlate, Nations::VnRect, confidence);
//                                 }          
//                                 else if(track.m_type == "VnSquare")
//                                 {
//                                     this->listPlateTracks[dist].license = readText(imgPlate, Nations::VnSquare, confidence);
//                                 }           
//                                 else if(track.m_type == "US")
//                                 {
//                                     this->listPlateTracks[dist].license = readText(imgPlate, Nations::US, confidence); 
//                                 } 
//                                 else if(track.m_type == "Malay")
//                                 {
//                                     this->listPlateTracks[dist].license = readText(imgPlate, Nations::MALAY, confidence); 
//                                 } 

//                                 if(!isnan(confidence))
//                                     this->listPlateTracks[dist].score = confidence;
//                                 else this->listPlateTracks[dist].score = 0;

//                                 if(this->listPlateTracks[dist].license == "Unknown") {
//                                     this->listPlateTracks[dist].countUnknown++;
//                                     this->listPlateTracks[dist].isNewEvent = false;
//                                 } else if(this->listPlateTracks[dist].license.size() < 7 || this->listPlateTracks[dist].license.size() > 10) {
//                                     this->listPlateTracks[dist].countUnknown++;
//                                     this->listPlateTracks[dist].isNewEvent = false;
//                                 } else {
//                                     this->listPlateTracks[dist].isDefectPlate = false;
//                                 }
//                             }
//                             else
//                             {
//                                 this->listPlateTracks[dist].license = "Unknown";
//                                 this->listPlateTracks[dist].score = 0;
//                                 // this->listPlateTracks[dist].countUnknown++;
//                                 this->listPlateTracks[dist].isNewEvent = false;
//                             }                            
//                         } 
//                         else if(this->listPlateTracks[dist].countUnknown == (MAX_COUNT_UNKNOWN + 1)) {
//                             this->listPlateTracks[dist].countUnknown++;
//                             this->listPlateTracks[dist].isDefectPlate = false;
//                             this->listPlateTracks[dist].isNewEvent = true;
//                         } 
//                         else {
//                             this->listPlateTracks[dist].isNewEvent = false;
//                         }

//                         if(this->listPlateTracks[dist].isNewEvent) {
//                             this->listPlateTracks[dist].licenseVec.push_back(this->listPlateTracks[dist].license);
//                             this->listPlateTracks[dist].isNewEvent = false;
//                             this->listPlateTracks[dist].countVerify += 1;
//                             if(this->listPlateTracks[dist].countVerify == MAX_COUNT_VERIFY) {
//                                 this->listPlateTracks[dist].countVerify += 1;
//                                 this->listPlateTracks[dist].isNewEvent = true;
//                             }
//                         }
                        
//                     }
//                     else  // if find new plate
//                     {
//                         cv::Mat imgPlate = img(track.m_rect).clone();
//                         float confidence = 0;
//                         PlateInfor plate;
//                         plate.imgPlate = imgPlate;
//                         // plate.isNewEvent = true;
//                         plate.track_id = track.m_ID;
//                         plate.rect = track.m_rect;
//                         plate.typePlate = track.m_type;   
//                         plate.isOutOfFrame = false;
//                         if(checkPlate(plate, img.cols, img.rows))
//                         {
//                             plate.isNewEvent = true;
//                             if(track.m_type == "VnRect")     
//                             {
//                                 plate.license = readText(imgPlate, Nations::VnRect, confidence);
//                             }          
//                             else if(track.m_type == "VnSquare")
//                             {
//                                 plate.license = readText(imgPlate, Nations::VnSquare, confidence);
//                             }           
//                             else if(track.m_type == "US")
//                             {
//                                 plate.license = readText(imgPlate, Nations::US, confidence); 
//                             } 
//                             else if(track.m_type == "Malay")
//                             {
//                                 plate.license = readText(imgPlate, Nations::MALAY, confidence); 
//                             }

//                             if(!isnan(confidence))
//                                 plate.score = confidence;
//                             else {
//                                 plate.score = 0;
//                                 // plate.license = "Unknown";
//                             }

//                             if(plate.license.size() < 7 || plate.license.size() > 10) {
//                                 plate.isDefectPlate = true;
//                                 plate.countUnknown = 1;
//                             }
//                         } else {
//                             plate.license = "Unknown";
//                             plate.score = 0;
//                             // plate.countUnknown = 1;
//                         } 

//                         if(plate.license == "Unknown" || plate.isDefectPlate) {
//                             plate.isNewEvent = false;
//                         }

//                         if(plate.isNewEvent) {
//                             plate.licenseVec.push_back(plate.license);
//                             plate.isNewEvent = false;
//                             plate.countVerify += 1;
//                         }

//                         this->listPlateTracks.push_back(plate);
//                     }
//                 }
//                 else
//                 {
//                     const int theId =  track.m_ID;
//                     const auto p = find_if(this->listPlateTracks.begin(), this->listPlateTracks.end(), 
//                                             [theId] ( const PlateInfor& a ) { return (a.track_id == theId);});                         
//                     if (p != this->listPlateTracks.end()) 
//                     {
//                         int dist = distance(this->listPlateTracks.begin(), p);
//                         this->listPlateTracks[dist].isOutOfFrame = true;
                        
//                     }
//                 } 
//             }  
//             //get list of output plates
//             for(auto p : this->listPlateTracks)
//             {
//                 if(p.isOutOfFrame != true) plates.push_back(p);
//             }                  
//         }
//         catch(const std::exception& e)
//         {
//             std::cerr << e.what() << '\n';
//         }
//     }
//     else
//     {
//         LOG_FAIL("Execute Anpr recognizer failed, please check your input");
//         return STATUS_FAILED; 
//     }
//     return STATUS_SUCCESS;
// }

int AnprRecognizer::trackVehicle(cv::Mat &img, cv::Mat &img_4K, std::vector<VehicleInfor>& vehicles)
{
    vehicles.clear();
    if(!img.empty() && !img_4K.empty())
    {
        int cols_4K = img_4K.cols;
        int rows_4K = img_4K.rows;
        try
        {   
            std::vector<ObjectTrace> objVehicles;
            if(this->vehicle_detector->executeObjectDetector(img, objVehicles, 0.4) != STATUS_SUCCESS)
            {
                LOG_FAIL("Execute Anpr detector failed");
                return STATUS_FAILED;
            }

            // detect
            std::vector<ObjectTrace> objPlates;
            this->detector->detect(img, objPlates);

            // process tracking
            std::vector<TrackingTrace> tracks;
            if(this->nations == Nations::VN) {
                this->tracker->process_anpr_VN(objPlates, tracks, 3.0, 10.0, 5.0, 7.0);
            } else {
                this->tracker->process_wh(objPlates, tracks, 5.0, 5.0);
            }

            //delete object which is abandoned
            for(auto it = this->listVehicleTracks.begin(); it != this->listVehicleTracks.end();)
            {
                const int theId =  (*it).plateInf.track_id;
                const auto p = find_if(tracks.begin(), tracks.end(), 
                                            [theId] ( const TrackingTrace& a ) { return (a.m_ID == theId);}); 
                if (p == tracks.end() && it != this->listVehicleTracks.end())
                    it = this->listVehicleTracks.erase(it);                
                else 
                    it++;
            }

            // process new track and update old track
            for(auto track : tracks)
            {
                if(!track.isOutOfFrame)
                {
                    const int theId =  track.m_ID;
                    const auto p = find_if(this->listVehicleTracks.begin(), this->listVehicleTracks.end(), 
                                            [theId] ( const VehicleInfor& a ) { return (a.plateInf.track_id == theId);});  
                    
                    if (p != this->listVehicleTracks.end()) // if find plate in list tracked plate
                    {
                        int dist = distance(this->listVehicleTracks.begin(), p);
                        this->listVehicleTracks[dist].plateInf.rect = track.m_rect;
                        this->listVehicleTracks[dist].plateInf.isOutOfFrame = false;
                        if((this->listVehicleTracks[dist].plateInf.license == "Unknown" || this->listVehicleTracks[dist].plateInf.isDefectPlate) 
                            && this->listVehicleTracks[dist].plateInf.countUnknown <= MAX_COUNT_UNKNOWN)
                        {
                            // this->listPlateTracks[dist].isNewEvent = true;
                            cv::Rect2f rect_4K = getRect4K(track.m_rect, cols_4K, rows_4K);
                            cv::Mat imgPlate4K = img_4K(rect_4K).clone();

                            cv::Mat imgPlate = img(this->listVehicleTracks[dist].plateInf.rect).clone();
                            float confidence = 0;
                            this->listVehicleTracks[dist].plateInf.imgPlate = imgPlate;
                            if(checkPlate(this->listVehicleTracks[dist].plateInf, img.cols, img.rows))
                            {
                                this->listVehicleTracks[dist].plateInf.isNewEvent = true;
                                if(track.m_type == "VnRect")     
                                {
                                    this->listVehicleTracks[dist].plateInf.license = readText(imgPlate4K, Nations::VnRect, confidence);
                                }          
                                else if(track.m_type == "VnSquare")
                                {
                                    this->listVehicleTracks[dist].plateInf.license = readText(imgPlate4K, Nations::VnSquare, confidence);
                                }           
                                else if(track.m_type == "US")
                                {
                                    this->listVehicleTracks[dist].plateInf.license = readText(imgPlate4K, Nations::US, confidence); 
                                } 
                                else if(track.m_type == "Malay")
                                {
                                    this->listVehicleTracks[dist].plateInf.license = readText(imgPlate4K, Nations::MALAY, confidence); 
                                } 
                                
                                if(!isnan(confidence))
                                    this->listVehicleTracks[dist].plateInf.score = confidence;
                                else this->listVehicleTracks[dist].plateInf.score = 0;

                                if(this->listVehicleTracks[dist].plateInf.license == "Unknown") {
                                    this->listVehicleTracks[dist].plateInf.countUnknown++;
                                    this->listVehicleTracks[dist].plateInf.isNewEvent = false;
                                } else if(this->listVehicleTracks[dist].plateInf.license.size() < 7 || this->listVehicleTracks[dist].plateInf.license.size() > 10) {
                                    this->listVehicleTracks[dist].plateInf.countUnknown++;
                                    this->listVehicleTracks[dist].plateInf.isNewEvent = false;
                                } else {
                                    this->listVehicleTracks[dist].plateInf.isDefectPlate = false;
                                }
                            }
                            else
                            {
                                this->listVehicleTracks[dist].plateInf.license = "Unknown";
                                this->listVehicleTracks[dist].plateInf.score = 0;
                                // this->listVehicleTracks[dist].plateInf.countUnknown++;
                                this->listVehicleTracks[dist].plateInf.isNewEvent = false;
                            }                            
                        } 
                        else if(this->listVehicleTracks[dist].plateInf.countUnknown == (MAX_COUNT_UNKNOWN + 1)) {
                            this->listVehicleTracks[dist].plateInf.countUnknown++;
                            this->listVehicleTracks[dist].plateInf.isDefectPlate = false;
                            this->listVehicleTracks[dist].plateInf.isNewEvent = true;
                        } 
                        else {
                            this->listVehicleTracks[dist].plateInf.isNewEvent = false;
                        }

                        if(this->listVehicleTracks[dist].plateInf.isNewEvent)
                            getAttribute(this->listVehicleTracks[dist], objVehicles);
                        
                    }
                    else  // if find new plate
                    {
                        cv::Rect2f rect_4K = getRect4K(track.m_rect, cols_4K, rows_4K);
                        cv::Mat imgPlate4K = img_4K(rect_4K).clone();

                        cv::Mat imgPlate = img(track.m_rect).clone();
                        float confidence = 0;
                        VehicleInfor vehicle;
                        vehicle.plateInf.imgPlate = imgPlate;
                        // vehicle.plateInf.isNewEvent = true;
                        vehicle.plateInf.track_id = track.m_ID;
                        vehicle.plateInf.rect = track.m_rect;
                        vehicle.plateInf.typePlate = track.m_type;   
                        vehicle.plateInf.isOutOfFrame = false;
                        if(checkPlate(vehicle.plateInf, img.cols, img.rows))
                        {
                            vehicle.plateInf.isNewEvent = true;
                            if(track.m_type == "VnRect")
                            {
                                vehicle.plateInf.license = readText(imgPlate4K, Nations::VnRect, confidence);
                            }          
                            else if(track.m_type == "VnSquare")
                            {
                                vehicle.plateInf.license = readText(imgPlate4K, Nations::VnSquare, confidence);
                            }           
                            else if(track.m_type == "US")
                            {
                                vehicle.plateInf.license = readText(imgPlate4K, Nations::US, confidence);
                            } 
                            else if(track.m_type == "Malay")
                            {
                                vehicle.plateInf.license = readText(imgPlate4K, Nations::MALAY, confidence);
                            }

                            if(!isnan(confidence))
                                vehicle.plateInf.score = confidence;
                            else vehicle.plateInf.score = 0;

                            if(vehicle.plateInf.license.size() < 7 || vehicle.plateInf.license.size() > 10) {
                                vehicle.plateInf.isDefectPlate = true;
                                vehicle.plateInf.countUnknown = 1;
                            }
                        } else {
                            vehicle.plateInf.license = "Unknown";
                            vehicle.plateInf.score = 0;
                            // vehicle.plateInf.countUnknown = 1;
                        } 

                        if(vehicle.plateInf.license == "Unknown" || vehicle.plateInf.isDefectPlate) {
                            vehicle.plateInf.isNewEvent = false;
                        }

                        if(vehicle.plateInf.isNewEvent)
                            getAttribute(vehicle, objVehicles);

                        this->listVehicleTracks.push_back(vehicle);
                    }
                }
                else
                {
                    const int theId =  track.m_ID;
                    const auto p = find_if(this->listVehicleTracks.begin(), this->listVehicleTracks.end(), 
                                            [theId] ( const VehicleInfor& a ) { return (a.plateInf.track_id == theId);});                         
                    if (p != this->listVehicleTracks.end()) 
                    {
                        int dist = distance(this->listVehicleTracks.begin(), p);
                        this->listVehicleTracks[dist].plateInf.isOutOfFrame = true;
                        
                    }
                } 
            }  
            //get list of output plates
            for(auto p : this->listVehicleTracks)
            {
                if(p.plateInf.isOutOfFrame != true) vehicles.push_back(p);
            }                  
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    else
    {
        LOG_FAIL("Execute Anpr recognizer failed, please check your input");
        return STATUS_FAILED; 
    }
    return STATUS_SUCCESS;
}

int AnprRecognizer::trackVehicle_lt(cv::Mat& mImage, cv::Mat& mImage4K, std::vector<VehicleInfor>& stVehicles)
{
    stVehicles.clear();

    if (mImage.empty() == false && mImage4K.empty() == false)
    {
        int iCols = mImage.cols;
        int iRows = mImage.rows;
        int iCols4K = mImage4K.cols;
        int iRows4K = mImage4K.rows;

        // detect vehicle
        std::vector<ObjectTrace> stObjectVehicles;
        if (this->vehicle_detector->executeObjectDetector(mImage, stObjectVehicles, 0.35) != STATUS_SUCCESS)
        {
            LOG_FAIL("Execute Vehicle detector failed");
            return STATUS_FAILED;
        }
        // tracking vehicle
        std::vector<TrackingTrace> stTrackVehicles;
        m_pVehicleTracker->process(stObjectVehicles, stTrackVehicles);

        // detect plate
        std::vector<ObjectTrace> stObjectPlates;
        if (this->detector->detect(mImage, stObjectPlates) != STATUS_SUCCESS)
        {
            LOG_FAIL("Execute Plate detector failed");
            return STATUS_FAILED;
        }
        // tracking plate
        std::vector<TrackingTrace> stTrackPlates;
        if (this->nations == Nations::VN)
        {
            this->tracker->process_anpr_VN(stObjectPlates, stTrackPlates, 3.0, 10.0, 5.0, 7.0);
        }
        else
        {
            this->tracker->process_wh(stObjectPlates, stTrackPlates, 5.0, 5.0);
        }

        // delete vehicle
        for (auto it = this->listVehicleTracks.begin(); it != this->listVehicleTracks.end(); )
        {
            const int iID = (*it).iTrackID;
            const auto p = find_if(stTrackVehicles.begin(), stTrackVehicles.end(), 
                                    [iID] (const TrackingTrace& a) { return (a.m_ID == iID); });
            if (p == stTrackVehicles.end() && it != this->listVehicleTracks.end())
            {
                it = this->listVehicleTracks.erase(it);
            }
            else
            {
                it ++;
            }
        }

        // update vehicle
        for (auto stTrack : stTrackVehicles)
        {
            if (stTrack.isOutOfFrame == false)
            {
                const int iID = stTrack.m_ID;
                const auto p = find_if(this->listVehicleTracks.begin(), this->listVehicleTracks.end(),
                                        [iID] (const VehicleInfor& a) { return (a.iTrackID == iID); });
                if (p != this->listVehicleTracks.end())
                {
                    int iIndex = distance(this->listVehicleTracks.begin(), p);
                    this->listVehicleTracks[iIndex].rectVehicle = stTrack.m_rect;
                    this->listVehicleTracks[iIndex].bOutOfFrame = false;

                    if (CheckOutside(stTrack, iCols, iRows))
                    {
                        this->listVehicleTracks[iIndex].uiCountGoOut += 1;
                    }
                }
                else
                {
                    VehicleInfor stVehicle;
                    stVehicle.iTrackID = stTrack.m_ID;
                    stVehicle.rectVehicle = stTrack.m_rect;
                    stVehicle.typeVehicle = stTrack.m_type;
                    stVehicle.bEvent = false;
                    this->listVehicleTracks.push_back(stVehicle);
                }
            }
            else
            {
                const int iID = stTrack.m_ID;
                const auto p = find_if(this->listVehicleTracks.begin(), this->listVehicleTracks.end(),
                                        [iID] (const VehicleInfor& a) { return (a.iTrackID == iID); });
                if (p != this->listVehicleTracks.end())
                {
                    int iIndex = distance(this->listVehicleTracks.begin(), p);
                    this->listVehicleTracks[iIndex].bOutOfFrame = true;
                }
            }
        }

        // delete plate
        for (auto it = this->listPlateTracks.begin(); it != this->listPlateTracks.end(); )
        {
            const int iID = (*it).track_id;
            const auto p = find_if(stTrackPlates.begin(), stTrackPlates.end(), 
                                    [iID] (const TrackingTrace& a) { return (a.m_ID == iID); });
            if (p == stTrackPlates.end() && it != this->listPlateTracks.end())
            {
                it = this->listPlateTracks.erase(it);
            }
            else
            {
                it ++;
            }
        }

        // update plate
        for (auto stTrack : stTrackPlates)
        {
            if (stTrack.isOutOfFrame == false)
            {
                const int iID = stTrack.m_ID;
                const auto p = find_if(this->listPlateTracks.begin(), this->listPlateTracks.end(), 
                                        [iID] (const PlateInfor& a) { return (a.track_id == iID); });
                if (p != this->listPlateTracks.end())
                {
                    int iIndex = distance(this->listPlateTracks.begin(), p);
                    this->listPlateTracks[iIndex].rect = stTrack.m_rect;
                    this->listPlateTracks[iIndex].isOutOfFrame = false;
                    if ((this->listPlateTracks[iIndex].license == "Unknown" || this->listPlateTracks[iIndex].isDefectPlate)
                        && this->listPlateTracks[iIndex].countUnknown <= MAX_COUNT_UNKNOWN)
                    {
                        cv::Rect2f rfBox4K = getRect4K(stTrack.m_rect, iCols4K, iRows4K);
                        cv::Mat mCropPlate4K = mImage4K(rfBox4K).clone();
                        cv::Mat mCropPlate = mImage(stTrack.m_rect).clone();
                        float fConfidence = 0;
                        this->listPlateTracks[iIndex].imgPlate = mCropPlate;
                        if (checkPlate(this->listPlateTracks[iIndex], iCols, iRows))
                        {
                            this->listPlateTracks[iIndex].isNewEvent = true;
                            if (stTrack.m_type == "VnRect")
                            {
                                this->listPlateTracks[iIndex].license = readText(mCropPlate4K, Nations::VnRect, fConfidence);
                            }
                            else if (stTrack.m_type == "VnSquare")
                            {
                                this->listPlateTracks[iIndex].license = readText(mCropPlate4K, Nations::VnSquare, fConfidence);
                            }
                            else if (stTrack.m_type == "US")
                            {
                                this->listPlateTracks[iIndex].license = readText(mCropPlate4K, Nations::US, fConfidence);
                            }
                            else if (stTrack.m_type == "Malay")
                            {
                                this->listPlateTracks[iIndex].license = readText(mCropPlate4K, Nations::MALAY, fConfidence);
                            }

                            if (isnan(fConfidence) == false)
                            {
                                this->listPlateTracks[iIndex].score = fConfidence;
                            }
                            else
                            {
                                this->listPlateTracks[iIndex].score = 0;
                            }

                            if (this->listPlateTracks[iIndex].license == "Unknown")
                            {
                                this->listPlateTracks[iIndex].countUnknown ++;
                                this->listPlateTracks[iIndex].isNewEvent = false;
                            }
                            else if (this->listPlateTracks[iIndex].license.size() < 7 || this->listPlateTracks[iIndex].license.size() > 10)
                            {
                                this->listPlateTracks[iIndex].countUnknown ++;
                                this->listPlateTracks[iIndex].isNewEvent = false;
                            }
                            else
                            {
                                this->listPlateTracks[iIndex].isDefectPlate = false;
                            }
                        }
                        else
                        {
                            this->listPlateTracks[iIndex].license = "Unknown";
                            this->listPlateTracks[iIndex].isNewEvent = false;
                        }
                    }
                    else if (this->listPlateTracks[iIndex].countUnknown == (MAX_COUNT_UNKNOWN + 1))
                    {
                        this->listPlateTracks[iIndex].countUnknown ++;
                        this->listPlateTracks[iIndex].isDefectPlate = false;
                        this->listPlateTracks[iIndex].isNewEvent = true;
                    }
                    else
                    {
                        this->listPlateTracks[iIndex].isNewEvent = false;
                    }

                    if (this->listPlateTracks[iIndex].isNewEvent)
                    {
                        MappingVehicle(this->listPlateTracks[iIndex], this->listVehicleTracks);
                    }
                    else
                    {
                        UpdateVehicle(this->listPlateTracks[iIndex], this->listVehicleTracks);
                    }
                }
                else
                {
                    cv::Rect2f rfBox4K = getRect4K(stTrack.m_rect, iCols4K, iRows4K);
                    cv::Mat mCropPlate4K = mImage4K(rfBox4K).clone();
                    cv::Mat mCropPlate = mImage(stTrack.m_rect).clone();
                    float fConfidence = 0;

                    PlateInfor stPlate;
                    stPlate.track_id = stTrack.m_ID;
                    stPlate.imgPlate = mCropPlate;
                    stPlate.rect = stTrack.m_rect;
                    stPlate.typePlate = stTrack.m_type;
                    stPlate.isOutOfFrame = false;
                    if (checkPlate(stPlate, iCols, iRows))
                    {
                        stPlate.isNewEvent = true;
                        if (stTrack.m_type == "VnRect")
                        {
                            stPlate.license = readText(mCropPlate4K, Nations::VnRect, fConfidence);
                        }
                        else if (stTrack.m_type == "VnSquare")
                        {
                            stPlate.license = readText(mCropPlate4K, Nations::VnSquare, fConfidence);
                        }
                        else if (stTrack.m_type == "US")
                        {
                            stPlate.license = readText(mCropPlate4K, Nations::US, fConfidence);
                        }
                        else if (stTrack.m_type == "Malay")
                        {
                            stPlate.license = readText(mCropPlate4K, Nations::MALAY, fConfidence);
                        }

                        if (isnan(fConfidence) == false)
                        {
                            stPlate.score = fConfidence;
                        }
                        else
                        {
                            stPlate.score = 0;
                        }

                        if (stPlate.license.size() < 7 || stPlate.license.size() > 10)
                        {
                            stPlate.isDefectPlate = true;
                            stPlate.countUnknown = 1;
                        }
                    }
                    else
                    {
                        stPlate.license = "Unknown";
                        stPlate.score = 0;
                    }

                    if (stPlate.license == "Unknown" || stPlate.isDefectPlate)
                    {
                        stPlate.isNewEvent = false;
                    }

                    if (stPlate.isNewEvent)
                    {
                        MappingVehicle(stPlate, this->listVehicleTracks);
                    }
                    else
                    {
                        UpdateVehicle(stPlate, this->listVehicleTracks);
                    }

                    this->listPlateTracks.push_back(stPlate);
                }
            }
            else
            {
                const int iID = stTrack.m_ID;
                const auto p = find_if(this->listPlateTracks.begin(), this->listPlateTracks.end(), 
                                        [iID] (const PlateInfor& a) { return (a.track_id == iID); });
                if (p != this->listPlateTracks.end())
                {
                    int iIndex = distance(this->listPlateTracks.begin(), p);
                    this->listPlateTracks[iIndex].isOutOfFrame = true;
                }
            }
        }

        for (auto& stVehicle : this->listVehicleTracks)
        {
            if (stVehicle.bOutOfFrame == false)
            {
                stVehicle.bEvent = false;
                if (stVehicle.uiCountGoOut == 1 && stVehicle.bHavePlate == false)
                {
                    stVehicle.bEvent = true;
                    stVehicle.uiCountGoOut += 1;
                }

                if (stVehicle.plateInf.isNewEvent)
                {
                    const int iPlateID = stVehicle.plateInf.track_id;
                    const auto p = find_if(this->listPlateTracks.begin(), this->listPlateTracks.end(), 
                                            [iPlateID] (const PlateInfor& a) { return (a.track_id == iPlateID); });
                    if (p != this->listPlateTracks.end())
                    {
                        int iIndex = distance(this->listPlateTracks.begin(), p);
                        if (this->listPlateTracks[iIndex].isOutOfFrame == false)
                        {
                            stVehicle.bEvent = true;
                            stVehicle.plateInf = this->listPlateTracks[iIndex];
                        }
                        else
                        {
                            stVehicle.bEvent = false;
                            PlateInfor stPlateEmpty;
                            stVehicle.plateInf = stPlateEmpty;
                        }
                    }
                }
                stVehicles.push_back(stVehicle);
            }
        }

    }
    else
    {
        LOG_FAIL("Execute ITS failed, please check your input");
        return STATUS_FAILED; 
    }

    return STATUS_SUCCESS;
}

bool AnprRecognizer::isValidPlate(cv::Mat& img)
{
    if(img.rows * img.cols >= 1500) 
        return true;
    return false;
}

bool AnprRecognizer::checkPlate(PlateInfor& plate, int cols, int rows)
{
    if(plate.rect.area() < 1500) return false;

    if((plate.rect.tl().x > 5) && (plate.rect.tl().y > 5) && (plate.rect.br().x < (cols - 5)) && (plate.rect.br().y < (rows - 5))) {
        return true;
    } else {
        return false;
    }
}

void AnprRecognizer::getAttribute(VehicleInfor& vehicle, std::vector<ObjectTrace> listVehicle)
{
    cv::Point2f cBox = cv::Point2f(vehicle.plateInf.rect.x + float(vehicle.plateInf.rect.width/2), 
                                        vehicle.plateInf.rect.y + float(vehicle.plateInf.rect.height/2));

    if(listVehicle.size() == 0) {
        vehicle.typeVehicle = "Unknown";
    }

    vehicle.typeVehicle = "Unknown";

    double max_distance = 0;

    for(int i=0; i<listVehicle.size(); i++) {
        std::vector<cv::Point2f> area{cv::Point2f(listVehicle[i].rect.x, listVehicle[i].rect.y + listVehicle[i].rect.height / 2), 
                                    cv::Point2f(listVehicle[i].rect.x + listVehicle[i].rect.width, listVehicle[i].rect.y + listVehicle[i].rect.height / 2), 
                                    listVehicle[i].rect.br(), 
                                    cv::Point2f(listVehicle[i].rect.x, listVehicle[i].rect.y + listVehicle[i].rect.height)};

        double distance = cv::pointPolygonTest(area, cBox, true);
        if(distance >= max_distance) {
            vehicle.rectVehicle = listVehicle[i].rect;
            vehicle.typeVehicle = listVehicle[i].label;
            
            max_distance = distance;
        }
    }
}

// void AnprRecognizer::getAttributes(cv::Mat& img, VehicleInfor& vehicle, std::vector<ObjectTrace> listVehicle)
// {
//     cv::Point2f cBox = cv::Point2f(vehicle.plateInf.rect.x + float(vehicle.plateInf.rect.width/2), 
//                                         vehicle.plateInf.rect.y + float(vehicle.plateInf.rect.height/2));

//     if(listVehicle.size() == 0) {
//         vehicle.typeVehicle = "Unknown";
//     }

//     vehicle.typeVehicle = "Unknown";

//     double max_distance = 0;

//     for(int i=0; i<listVehicle.size(); i++) {
//         std::vector<cv::Point2f> area{cv::Point2f(listVehicle[i].rect.x, listVehicle[i].rect.y + listVehicle[i].rect.height / 2), 
//                                     cv::Point2f(listVehicle[i].rect.x + listVehicle[i].rect.width, listVehicle[i].rect.y + listVehicle[i].rect.height / 2), 
//                                     listVehicle[i].rect.br(), 
//                                     cv::Point2f(listVehicle[i].rect.x, listVehicle[i].rect.y + listVehicle[i].rect.height)};

//         double distance = cv::pointPolygonTest(area, cBox, true);
//         if(distance >= max_distance) {
//             vehicle.rectVehicle = listVehicle[i].rect;
//             vehicle.typeVehicle = listVehicle[i].label;
            
//             max_distance = distance;
//         }
//     }

//     if(vehicle.typeVehicle != "Unknown") {
//         if(find(listGetAttributes.begin(), listGetAttributes.end(), vehicle.typeVehicle) != listGetAttributes.end()) {
//             vehicle.imgVehicle = img(vehicle.rectVehicle).clone();
//             std::cout << "--------------------" << vehicle.imgVehicle.size() << " | " << vehicle.rectVehicle << std::endl;
//             this->vehicle_attributes->executeObjectAttributes(vehicle.imgVehicle, vehicle.attributes);
//         }
//     }
    
// }

bool AnprRecognizer::CheckOutside(TrackingTrace& stTrack, int iWidthImage, int iHeightImage)
{
    if (stTrack.m_trace.size() < 2)
    {
        return false;
    }
    else
    {
        cv::Rect2f rfBox = cv::Rect2f(cv::Point2f(iWidthImage * 0.15f, iHeightImage * 0.15f), 
                                        cv::Point2f(iWidthImage * 0.85f, iHeightImage * 0.85f));
        
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

void AnprRecognizer::MappingVehicle(PlateInfor& stPlate, std::vector<VehicleInfor>& stVehicleTracks)
{
    cv::Point2f ptCenterBox = cv::Point2f(stPlate.rect.x + (float)stPlate.rect.width / 2, stPlate.rect.y + (float)stPlate.rect.height / 2);
    double dMaxDistance = 0;
    cv::Rect2i riBox;
    std::string strType;
    for (auto& stVehicle : stVehicleTracks)
    {
        if (stVehicle.bOutOfFrame == false && stVehicle.bEvent == false)
        {
            std::vector<cv::Point2f> ptAreas{cv::Point2f(stVehicle.rectVehicle.x, (float)stVehicle.rectVehicle.y + stVehicle.rectVehicle.height / 2), 
                                            cv::Point2f(stVehicle.rectVehicle.x + stVehicle.rectVehicle.width, stVehicle.rectVehicle.y + stVehicle.rectVehicle.height / 2),
                                            cv::Point2f(stVehicle.rectVehicle.br()), 
                                            cv::Point2f(stVehicle.rectVehicle.x, stVehicle.rectVehicle.height)};
            double dDistance = cv::pointPolygonTest(ptAreas, ptCenterBox, true);
            if (dDistance >= dMaxDistance)
            {
                stVehicle.dDistance = dDistance;
                dMaxDistance = dDistance;
                riBox = stVehicle.rectVehicle;
                strType = stVehicle.typeVehicle;
            }
        }
    }

    if (dMaxDistance > 0)
    {
        const auto p = find_if(stVehicleTracks.begin(), stVehicleTracks.end(), 
                                [dMaxDistance] (const VehicleInfor& a) { return (a.dDistance == dMaxDistance); });
        if (p != stVehicleTracks.end())
        {
            int iIndex = distance(stVehicleTracks.begin(), p);
            stVehicleTracks[iIndex].rectVehicle = riBox;
            stVehicleTracks[iIndex].typeVehicle = strType;
            stVehicleTracks[iIndex].plateInf = stPlate;
            stVehicleTracks[iIndex].bHavePlate = true;
        }
    }
    else
    {
        VehicleInfor stVehicle;
        stVehicle.typeVehicle = "Unknown";
        stVehicle.plateInf = stPlate;
        stVehicle.bHavePlate = true;
        stVehicleTracks.push_back(stVehicle);
    }
}

void AnprRecognizer::UpdateVehicle(PlateInfor& stPlate, std::vector<VehicleInfor>& stVehicleTracks)
{
    const int iPlateID = stPlate.track_id;
    const auto p = find_if(stVehicleTracks.begin(), stVehicleTracks.end(), 
                            [iPlateID] (const VehicleInfor& a) { return (a.plateInf.track_id == iPlateID); });
    if (p != stVehicleTracks.end())
    {
        int iIndex = distance(stVehicleTracks.begin(), p);
        stVehicleTracks[iIndex].plateInf = stPlate;
    }
}

// 
pthread_t vehicle_thread;
struct arg_VehicleDetect arg_vehicle;

void *thread_VehicleDetect(void *args)
{
    pthread_detach(pthread_self());
    struct arg_VehicleDetect *arg = (struct arg_VehicleDetect*) args;
    arg->p->vehicle_detector->executeObjectDetector(arg->img, arg->objVehicles, 0.4);
    arg->p->objVehicles = arg->objVehicles;
    pthread_exit(NULL);
}

pthread_t plate_thread;
struct arg_PlateTracking arg_plate;

void *thread_PlateTracking(void *args)
{
    // pthread_detach(pthread_self());
    struct arg_PlateTracking *arg = (struct arg_PlateTracking*) args;
    std::vector<ObjectTrace> objPlates;
    arg->p->detector->detect(arg->img, objPlates);

    std::vector<TrackingTrace> tracks;
    // arg->p->tracker->process(objPlates, tracks, 5.0);
    if(vehicle_nations == Nations::VN) {
        arg->p->tracker->process_anpr_VN(objPlates, tracks, 3.0, 10.0, 5.0, 7.0);
    } else {
        arg->p->tracker->process_wh(objPlates, tracks, 5.0, 5.0);
    }

    for(auto it = arg->p->listVehicleTracks.begin(); it != arg->p->listVehicleTracks.end();)
    {
        const int theId =  (*it).plateInf.track_id;
        const auto p = find_if(tracks.begin(), tracks.end(), 
                                    [theId] ( const TrackingTrace& a ) { return (a.m_ID == theId);}); 
        if (p == tracks.end() && it != arg->p->listVehicleTracks.end())
            it = arg->p->listVehicleTracks.erase(it);                
        else 
            it++;
    }

    arg->p->tracks = tracks;
    pthread_exit(NULL);
}

int AnprRecognizer::threadVehicle(cv::Mat &img, cv::Mat &img_4K, std::vector<VehicleInfor>& vehicles)
{
    if(!img.empty() && !img_4K.empty())
    {
        int cols_4K = img_4K.cols;
        int rows_4K = img_4K.rows;
        try
        {
            arg_vehicle.p = this;
            arg_vehicle.img = img;
            pthread_create(&vehicle_thread, NULL, &thread_VehicleDetect, &arg_vehicle);

            arg_plate.p = this;
            arg_plate.img = img;
            pthread_create(&plate_thread, NULL, &thread_PlateTracking, &arg_plate);
            pthread_join(plate_thread, NULL);

            for(auto track : this->tracks)
            {
                if(!track.isOutOfFrame)
                {
                    const int theId =  track.m_ID;
                    const auto p = find_if(this->listVehicleTracks.begin(), this->listVehicleTracks.end(), 
                                            [theId] ( const VehicleInfor& a ) { return (a.plateInf.track_id == theId);});  
                    
                    if (p != this->listVehicleTracks.end()) // if find plate in list tracked plate
                    {
                        int dist = distance(this->listVehicleTracks.begin(), p);
                        this->listVehicleTracks[dist].plateInf.rect = track.m_rect;
                        this->listVehicleTracks[dist].plateInf.isOutOfFrame = false;
                        if((this->listVehicleTracks[dist].plateInf.license == "Unknown" || this->listVehicleTracks[dist].plateInf.isDefectPlate) 
                            && this->listVehicleTracks[dist].plateInf.countUnknown <= MAX_COUNT_UNKNOWN)
                        {
                            // this->listPlateTracks[dist].isNewEvent = true;
                            cv::Rect2f rect_4K = getRect4K(track.m_rect, cols_4K, rows_4K);
                            cv::Mat imgPlate4K = img_4K(rect_4K).clone();

                            cv::Mat imgPlate = img(this->listVehicleTracks[dist].plateInf.rect).clone();
                            float confidence = 0;
                            this->listVehicleTracks[dist].plateInf.imgPlate = imgPlate;
                            if(checkPlate(this->listVehicleTracks[dist].plateInf, img.cols, img.rows))
                            {
                                this->listVehicleTracks[dist].plateInf.isNewEvent = true;
                                if(track.m_type == "VnRect")     
                                {
                                    this->listVehicleTracks[dist].plateInf.license = readText(imgPlate4K, Nations::VnRect, confidence);
                                }          
                                else if(track.m_type == "VnSquare")
                                {
                                    this->listVehicleTracks[dist].plateInf.license = readText(imgPlate4K, Nations::VnSquare, confidence);
                                }           
                                else if(track.m_type == "US")
                                {
                                    this->listVehicleTracks[dist].plateInf.license = readText(imgPlate4K, Nations::US, confidence); 
                                } 
                                else if(track.m_type == "Malay")
                                {
                                    this->listVehicleTracks[dist].plateInf.license = readText(imgPlate4K, Nations::MALAY, confidence); 
                                } 
                                // else {
                                //     this->listVehicleTracks[dist].plateInf.license = readText(imgPlate, Nations::VnSquare, confidence);
                                // }
                                if(!isnan(confidence))
                                    this->listVehicleTracks[dist].plateInf.score = confidence;
                                else this->listVehicleTracks[dist].plateInf.score = 0;

                                if(this->listVehicleTracks[dist].plateInf.license == "Unknown") {
                                    this->listVehicleTracks[dist].plateInf.countUnknown++;
                                    this->listVehicleTracks[dist].plateInf.isNewEvent = false;
                                } else if(this->listVehicleTracks[dist].plateInf.license.size() < 7 || this->listVehicleTracks[dist].plateInf.license.size() > 10) {
                                    this->listVehicleTracks[dist].plateInf.countUnknown++;
                                    this->listVehicleTracks[dist].plateInf.isNewEvent = false;
                                } else {
                                    this->listVehicleTracks[dist].plateInf.isDefectPlate = false;
                                }
                            }
                            else
                            {
                                this->listVehicleTracks[dist].plateInf.license = "Unknown";
                                this->listVehicleTracks[dist].plateInf.score = 0;
                                // this->listVehicleTracks[dist].plateInf.countUnknown++;
                                this->listVehicleTracks[dist].plateInf.isNewEvent = false;
                            }                            
                        } 
                        else if(this->listVehicleTracks[dist].plateInf.countUnknown == (MAX_COUNT_UNKNOWN + 1)) {
                            this->listVehicleTracks[dist].plateInf.countUnknown++;
                            this->listVehicleTracks[dist].plateInf.isDefectPlate = false;
                            this->listVehicleTracks[dist].plateInf.isNewEvent = true;
                        } 
                        else {
                            this->listVehicleTracks[dist].plateInf.isNewEvent = false;
                        } 

                        if(this->listVehicleTracks[dist].plateInf.isNewEvent)
                            getAttribute(this->listVehicleTracks[dist], objVehicles);
                        
                    }
                    else  // if find new plate
                    {
                        cv::Rect2f rect_4K = getRect4K(track.m_rect, cols_4K, rows_4K);
                        cv::Mat imgPlate4K = img_4K(rect_4K).clone();

                        cv::Mat imgPlate = img(track.m_rect).clone();
                        float confidence = 0;
                        VehicleInfor vehicle;
                        vehicle.plateInf.imgPlate = imgPlate;
                        // vehicle.plateInf.isNewEvent = true;
                        vehicle.plateInf.track_id = track.m_ID;
                        vehicle.plateInf.rect = track.m_rect;
                        vehicle.plateInf.typePlate = track.m_type;
                        vehicle.plateInf.isOutOfFrame = false;
                        if(checkPlate(vehicle.plateInf, img.cols, img.rows))
                        {
                            vehicle.plateInf.isNewEvent = true;
                            if(track.m_type == "VnRect")     
                            {
                                vehicle.plateInf.license = readText(imgPlate4K, Nations::VnRect, confidence);
                            }          
                            else if(track.m_type == "VnSquare")
                            {
                                vehicle.plateInf.license = readText(imgPlate4K, Nations::VnSquare, confidence);
                            }           
                            else if(track.m_type == "US")
                            {
                                vehicle.plateInf.license = readText(imgPlate4K, Nations::US, confidence); 
                            } 
                            else if(track.m_type == "Malay")
                            {
                                vehicle.plateInf.license = readText(imgPlate4K, Nations::MALAY, confidence); 
                            } 
                            // else {
                            //     vehicle.plateInf.license = readText(imgPlate, Nations::VnSquare, confidence);
                            // }
                            if(!isnan(confidence))
                                vehicle.plateInf.score = confidence;
                            else vehicle.plateInf.score = 0;

                            if(vehicle.plateInf.license.size() < 7 || vehicle.plateInf.license.size() > 10) {
                                vehicle.plateInf.isDefectPlate = true;
                                vehicle.plateInf.countUnknown = 1;
                            }
                        } else {
                            vehicle.plateInf.license = "Unknown";
                            vehicle.plateInf.score = 0;
                            // vehicle.plateInf.countUnknown = 1;
                        } 

                        if(vehicle.plateInf.license == "Unknown" || vehicle.plateInf.isDefectPlate) {
                            vehicle.plateInf.isNewEvent = false;
                        }

                        if(vehicle.plateInf.isNewEvent)
                            getAttribute(vehicle, objVehicles);

                        this->listVehicleTracks.push_back(vehicle);
                    }
                }
                else
                {
                    const int theId =  track.m_ID;
                    const auto p = find_if(this->listVehicleTracks.begin(), this->listVehicleTracks.end(), 
                                            [theId] ( const VehicleInfor& a ) { return (a.plateInf.track_id == theId);});                         
                    if (p != this->listVehicleTracks.end()) 
                    {
                        int dist = distance(this->listVehicleTracks.begin(), p);
                        this->listVehicleTracks[dist].plateInf.isOutOfFrame = true;
                        
                    }
                } 
            }  
            //get list of output plates
            for(auto p : this->listVehicleTracks)
            {
                if(this->objectsTrack.size() == 0) {
                    p.onObjectsTrack = true;
                } else {
                    if(find(this->objectsTrack.begin(), this->objectsTrack.end(), p.typeVehicle) != this->objectsTrack.end()) {
                        p.onObjectsTrack = true;
                    } else {
                        p.onObjectsTrack = false;
                    }
                }
                if((p.plateInf.isOutOfFrame != true) && (p.onObjectsTrack == true)) vehicles.push_back(p);
            }

        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    else
    {
        LOG_FAIL("Execute Anpr recognizer failed, please check your input");
        return STATUS_FAILED; 
    }
    return STATUS_SUCCESS;
}
