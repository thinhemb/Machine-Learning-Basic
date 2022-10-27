/******************************************************************************** 
 Module: SnpeRuntime.cpp
 Author: LE MANH THANG
 Created: 08/02/2021
 Description: 
********************************************************************************/
#include "SnpeMobilenetSSD.hpp"


SnpeMobilenetSSD::SnpeMobilenetSSD(/* args */)
{
    this->snpeMobilenetSSD = new snpeBuilders;
}

SnpeMobilenetSSD::~SnpeMobilenetSSD()
{
    if(this->snpeMobilenetSSD) 
        delete this->snpeMobilenetSSD;
}
int SnpeMobilenetSSD::initSnpeMobilenetSSD(std::string containerPath)
{
    std::vector<std::string> outputLayers = {"add", "Postprocessor/BatchMultiClassNonMaxSuppression"};
    this->snpeMobilenetSSD->runtime = SNPE_RUNTIME;

    
    this->snpeMobilenetSSD->snpe = setBuilderSNPE(containerPath, outputLayers, this->snpeMobilenetSSD->runtime);
    if (this->snpeMobilenetSSD->snpe  == nullptr)
    {
       std::cerr << "Error while building SNPE object." << std::endl;
       //return nullptr;
       return -1;
    }
    LOG_SUCCESS("Configured MobilenetSSD network successfully\n");
    return 0;
}
int SnpeMobilenetSSD::initSnpeMobilenetSSD(std::string containerPath, std::string targetDevice)
{
    std::vector<std::string> outputLayers = {"add", "Postprocessor/BatchMultiClassNonMaxSuppression"};
    this->snpeMobilenetSSD->runtime = SNPE_RUNTIME;
    if(targetDevice == "DSP")
    {
        this->snpeMobilenetSSD->runtime = zdl::DlSystem::Runtime_t::DSP;
        LOG_INFO_("SNPE run on DSP");
    }
    else if (targetDevice == "GPU")
    {
        this->snpeMobilenetSSD->runtime = zdl::DlSystem::Runtime_t::GPU;
        LOG_INFO_("SNPE run on GPU");
    }
    else 
        this->snpeMobilenetSSD->runtime = zdl::DlSystem::Runtime_t::CPU;
    
    this->snpeMobilenetSSD->snpe = setBuilderSNPE(containerPath, outputLayers, this->snpeMobilenetSSD->runtime);
    if (this->snpeMobilenetSSD->snpe  == nullptr)
    {
       std::cerr << "Error while building SNPE object." << std::endl;
       //return nullptr;
       return -1;
    }
    LOG_SUCCESS("Configured MobilenetSSD network successfully");
    return 0;
}

int SnpeMobilenetSSD::executeSnpeMobilenetSSD(const cv::Mat& img, std::vector<std::string>& labels, std::vector<ObjectTrace>& objects, float thres_detect)
{
    try
    {
        //Execute the network and store the outputs that were specified when creating the network in a TensorMap
        std::unique_ptr<zdl::DlSystem::ITensor> input;
        input = convertMat2BgrFloat(this->snpeMobilenetSSD->snpe, img);
        static zdl::DlSystem::TensorMap outputTensorMap;
        int exeStatus  = this->snpeMobilenetSSD->snpe->execute(input.get(), outputTensorMap);
        if(exeStatus != true)
        {
            printf("Error while executing the MobilenetSSD network \n");
            return -1;
        }

        zdl::DlSystem::StringList tensorNames = outputTensorMap.getTensorNames();

        std::string scoresName = "Postprocessor/BatchMultiClassNonMaxSuppression_scores";
        std::string classesName = "detection_classes:0";
        std::string boxesName = "Postprocessor/BatchMultiClassNonMaxSuppression_boxes";

        zdl::DlSystem::ITensor *outTensorScores = outputTensorMap.getTensor(scoresName.c_str());
        zdl::DlSystem::ITensor *outTensorClasses = outputTensorMap.getTensor(classesName.c_str());
        zdl::DlSystem::ITensor *outTensorBoxes = outputTensorMap.getTensor(boxesName.c_str());
        zdl::DlSystem::TensorShape scoresShape = outTensorScores->getShape();
        
        if(scoresShape.rank() != 2) {
            std::cerr << "Scores should have two axis" << std::endl;
            return -1;
        }
        const auto *oScores = reinterpret_cast<float*>(&(*outTensorScores->begin()));
        const auto *oClasses = reinterpret_cast<float*>(&(*outTensorClasses->begin()));
        const auto *oBoxes = reinterpret_cast<float*>(&(*outTensorBoxes->begin()));

        std::vector<int> boxes;
        std::vector<int> classes;
        objects.clear();
        for(size_t curProposal = 0; curProposal < scoresShape[1]; curProposal++) 
        {
            if(oScores[curProposal] >= thres_detect)
            {
                ObjectTrace obj;
                obj.obj_id = static_cast<int>(oClasses[curProposal] - 1);
                obj.label = labels[static_cast<int>(oClasses[curProposal] - 1)];
                obj.score = oScores[curProposal];
                
                obj.rect.x = static_cast<int>(oBoxes[4 * curProposal + 1] * img.cols);
                obj.rect.y = static_cast<int>(oBoxes[4 * curProposal] * img.rows);
                obj.rect.width = static_cast<int>(oBoxes[4 * curProposal + 3] * img.cols) - obj.rect.x;
                obj.rect.height = static_cast<int>(oBoxes[4 * curProposal + 2] * img.rows) - obj.rect.y;

                if(obj.rect.x < 0) obj.rect.x  = 0;
                if(obj.rect.y < 0) obj.rect.y  = 0;
                if((obj.rect.x + obj.rect.width) >= img.cols) obj.rect.width = img.cols - obj.rect.x;
                if((obj.rect.y + obj.rect.height) >= img.rows) obj.rect.height = img.rows - obj.rect.y;
                if(obj.rect.width < 0 || obj.rect.height < 0) continue;
                if(obj.rect.area() > (img.cols * img.rows * 0.55)) continue; 
                objects.push_back(obj);
            }
        }
    }
    catch (const std::exception& error) 
    {
        // std::cout << "Execute snpe runtime error!\n";
        return -1;
    }
    return 0;
}
