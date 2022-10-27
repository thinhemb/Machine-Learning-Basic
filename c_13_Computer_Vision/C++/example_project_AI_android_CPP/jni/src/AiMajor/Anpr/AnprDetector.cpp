/*
    Module: AnprDetector.cpp
    Author: ThangLmb
    Date: June 24, 2021
*/
#include "AnprDetector.hpp"

AnprDetector::AnprDetector(/* args */)
{
    this->obj_detector = new ObjectDetector();
    #if(VEHICLE_DETECTION == 1)
        VehicleDetector* vehicle = nullptr;
    #endif
}

AnprDetector::~AnprDetector()
{
    if(this->obj_detector != nullptr) delete this->obj_detector;
    #if(VEHICLE_DETECTION == 1)
        if(this->vihecle != nullptr) delete this->vihecle;
    #endif
}

int AnprDetector::init(Nations nation)
{
    if(nation == Nations::VN)
    {
        if(!this->obj_detector->initObjectDetector("snpe", "AnprDetect", "DSP"))
            return STATUS_FAILED;
    }
    else if(nation == Nations::US)  
    {
        // if(!this->obj_detector->initObjectDetector("ncnn", "AnprDetect_US", "CPU"))
        if(!this->obj_detector->initObjectDetector("snpe", "usplate", "DSP"))
            return STATUS_FAILED;
    }
    else if(nation == Nations::MALAY)  
    {
        if(!this->obj_detector->initObjectDetector("ncnn", "AnprDetect_Malay", "CPU"))
            return STATUS_FAILED;
    }
    else if (nation == Nations::China)
    {
        if (this->obj_detector->initObjectDetector("snpe", "chinaplate", "DSP") != STATUS_SUCCESS)
        {
            return STATUS_FAILED;
        }
    }

    return STATUS_SUCCESS;
}

int AnprDetector::detect ( cv::Mat& img, std::vector<ObjectTrace>& objects)
{
    if(!img.empty())
    {
        #if(VEHICLE_DETECTION == 1)
        cv::Mat srcImg;
        img.copyTo(srcImg);
        objects.clear();
        std::vector<ObjectTrace> vehicles;
        this->vehicle->detect(img, vehicles);
        std::cout << "Number of vehicles: " << vehicles.size() << std::endl;
        for(int i = 0; i < vehicles.size(); i++)
        {
            // if(vehicles[i].label == "otocon" || vehicles[i].label == "oto con")
            // {

                cv::Mat imgVehicle = img(vehicles[i].rect).clone();
                float w_ratio = img.cols / imgVehicle.cols;
                float h_ratio = img.rows / imgVehicle.rows;
                
                std::vector<ObjectTrace> plates;
                this->obj_detector->executeObjectDetector(imgVehicle, plates, 0.3);
                for(int j = 0; j < plates.size(); j++)
                {
                    ObjectTrace plate = plates[j];
                    plate.rect.x = vehicles[i].rect.x + plate.rect.x;
                    plate.rect.y = vehicles[i].rect.y +  plate.rect.y;
                    plate.rect.width = plate.rect.width;
                    plate.rect.height = plate.rect.height;
                    objects.push_back(plate);
                }
                cv::rectangle(srcImg, vehicles[i].rect, cv::Scalar(255, 0, 0), 1, 8);
            //}
        }
        img = srcImg;
        //cv::imwrite("/data/thanglmb/vehicle.jpg", srcImg);
        #else 
            if(this->obj_detector->executeObjectDetector(img, objects, THRES_DETECT) != STATUS_SUCCESS)
            {
                LOG_FAIL("Execute Anpr detector faield");
                return STATUS_FAILED;
            }
        #endif

        for(int i = 0; i < objects.size(); i++)
        {
            objects[i].rect.x -=  objects[i].rect.width * 0.1;
            objects[i].rect.x = objects[i].rect.x >= 0 ? objects[i].rect.x : 0;

            objects[i].rect.y -=  objects[i].rect.height * 0.1;
            objects[i].rect.y = objects[i].rect.y >= 0 ? objects[i].rect.y : 0;

            objects[i].rect.width +=  objects[i].rect.width * 0.3;
            objects[i].rect.width = (objects[i].rect.x + objects[i].rect.width) > img.cols
            ?
            (img.cols - objects[i].rect.x) : objects[i].rect.width;

            objects[i].rect.height +=  objects[i].rect.height * 0.3;
            objects[i].rect.height = (objects[i].rect.y + objects[i].rect.height) > img.rows
            ?
            (img.rows - objects[i].rect.y) : objects[i].rect.height;
        }
    }
    else 
    {
        LOG_FAIL("Execute Anpr detector failed, please check your input");
        return STATUS_FAILED;
    }
    return STATUS_SUCCESS;
}
