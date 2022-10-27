#include "ObjectDetector.hpp"
#include "LoadLabel.hpp"

ObjectDetector::ObjectDetector()
{

}

ObjectDetector::~ObjectDetector()
{
    // if(this->tflite_runtime != nullptr)   delete this->tflite_runtime;
    if(this->snpe_detector != nullptr)    delete this->snpe_detector;
    // if(this->ncnn_detector != nullptr)    delete  this->ncnn_detector;
    // if(this->mnn_detector != nullptr)    delete this->mnn_detector;
}

int ObjectDetector::initObjectDetector(std::string framework, std::string objectTarget)
{
    this->framework = framework;
    this->objectTarget = objectTarget;
    
    // init framework snpe
    if(this->framework == "snpe")
    {
        this->snpe_detector = new SnpeDetector();
        // init model
        std::string path_model = DIR_MODEL_OBJECTDETECION_SNPE + this->objectTarget + ".dlc";
        std::cout << "[INFO]- Snpe path model: " << path_model << std::endl;
        if(this->snpe_detector->initSnpeObjectDetector(path_model) != STATUS_SUCCESS)
        {
            printf("Init SNPE object detector network error\n");
            return STATUS_FAILED;
        }
        // load label
        std::string path_label = DIR_LABEL_OBJECTDETECION_SNPE + this->objectTarget + ".txt";
        std::cout << "[INFO]- Snpe path label: " << path_label << std::endl;
        if(loadLabel(path_label, labels) != STATUS_SUCCESS)
        {
            LOG_FAIL("Load labels error");
            return STATUS_FAILED;
        }    
    }
    else if(this->framework == "tflite")
    {
        // this->tflite_runtime = new TfLiteRuntime();
        // // init model
        // std::string path_model = DIR_MODEL_OBJECTDETECION_TFLITE + this->objectTarget + ".tflite";
        // char path[100];
        // strcpy(path, path_model.c_str());
        // std::cout << "[INFO]- Snpe path model: " << path << std::endl;
        // if(this->tflite_runtime->initTfLiteNetwork(path, "GPU") != STATUS_SUCCESS)
        // {
        //     printf("Init SNPE object detector network error\n");
        //     return STATUS_FAILED;
        // }
        // // load label
        // std::string path_label = DIR_LABEL_OBJECTDETECION_TFLITE + this->objectTarget + ".txt";
        // std::cout << "[INFO]- Snpe path label: " << path_label << std::endl;
        // if(loadLabel(path_label, labels) != STATUS_SUCCESS)
        // {
        //     LOG_FAIL("Load labels error");
        //     return STATUS_FAILED;
        // } 
    }
    else if(this->framework == "ncnn")
    {
        // this->ncnn_detector = new NcnnDetector();
        // // init model
        // std::string path_model_bin = DIR_MODEL_OBJECTDETECION_NCNN + this->objectTarget + ".bin";
        // std::string path_model_param = DIR_MODEL_OBJECTDETECION_NCNN + this->objectTarget + ".param";
        // std::cout << "[INFO]- Snpe path model: " << path_model_bin <<  ", " << path_model_param << std::endl;
        // char path_bin[100];
        // strcpy(path_bin, path_model_bin.c_str());
        // char path_param[100];
        // strcpy(path_param, path_model_param.c_str());
        // if(this->ncnn_detector->initNcnnNetwork(path_bin, path_param) != STATUS_SUCCESS)
        // {
        //     printf("Init ncnn network error\n");
        //     return STATUS_FAILED;
        // }
        // // load label
        // std::string path_label = DIR_LABEL_OBJECTDETECION_NCNN + this->objectTarget + ".txt";
        // std::cout << "[INFO]- ncnn path label: " << path_label << std::endl;
        // if(loadLabel(path_label, labels) != STATUS_SUCCESS)
        // {
        //     LOG_FAIL("Load labels error");
        //     return STATUS_FAILED;
        // }
    }
    else if(this->framework == "mnn")
    {
        // this->mnn_detector = new MnnDetector();
        // // init model
        // std::string path_model = DIR_LABEL_OBJECTDETECION_MNN + this->objectTarget + ".mnn";
        // std::cout << "[INFO]- MNN path model: " << path_model << std::endl;
        // if(this->mnn_detector->init(path_model.c_str()) != STATUS_SUCCESS)
        // {
        //     LOG_FAIL("Load labels error");
        //     return STATUS_FAILED;
        // }
        // // load label
        // std::string path_label = DIR_LABEL_OBJECTDETECION_MNN + this->objectTarget + ".txt";
        // std::cout << "[INFO]- MNN path label: " << path_label << std::endl;
        // if(loadLabel(path_label, labels) != STATUS_SUCCESS)
        // {
        //     LOG_FAIL("Load labels error");
        //     return STATUS_FAILED;
        // }
    }
    LOG_SUCCESS("Init Object Detector successfully");
    return STATUS_SUCCESS;
}
int ObjectDetector::initObjectDetector(std::string framework, std::string objectTarget, std::string deviceTarget)
{
    this->framework = framework;
    this->objectTarget = objectTarget;
    if(this->framework == "snpe")
    {
        this->snpe_detector = new SnpeDetector();
        // init model
        std::string path_model = DIR_MODEL_OBJECTDETECION_SNPE + this->objectTarget + ".dlc";
        std::cout << "[INFO]- Snpe path model: " << path_model << std::endl;
        if(this->snpe_detector->initSnpeObjectDetector(path_model, deviceTarget) != STATUS_SUCCESS)
        {
            printf("Init SNPE object detector network error\n");
            return STATUS_FAILED;
        }
        // load label
        std::string path_label = DIR_LABEL_OBJECTDETECION_SNPE + this->objectTarget + ".txt";
        std::cout << "[INFO]- Snpe path label: " << path_label << std::endl;
        if(loadLabel(path_label, labels) != STATUS_SUCCESS)
        {
            LOG_FAIL("Load labels error");
            return STATUS_FAILED;
        }     
    }

    else if(this->framework == "ncnn")
    {
        // this->ncnn_detector = new NcnnDetector();
        // // init model
        // std::string path_model_bin = DIR_MODEL_OBJECTDETECION_NCNN + this->objectTarget + ".bin";
        // std::string path_model_param = DIR_MODEL_OBJECTDETECION_NCNN + this->objectTarget + ".param";

        // std::cout << "[INFO]- Snpe path model: " << path_model_bin <<  ", " << path_model_param << std::endl;
        // char path_bin[100];
        // strcpy(path_bin, path_model_bin.c_str());
        // char path_param[100];
        // strcpy(path_param, path_model_param.c_str());
        // if(this->ncnn_detector->initNcnnNetwork(path_bin, path_param, deviceTarget) != STATUS_SUCCESS)
        // {
        //     LOG_FAIL("Init ncnn network error");
        //     return STATUS_FAILED;
        // }
        // // load label
        // std::string path_label = DIR_LABEL_OBJECTDETECION_NCNN + this->objectTarget + ".txt";
        // std::cout << "[INFO]- ncnn path label: " << path_label << std::endl;
        // if(loadLabel(path_label, labels) != STATUS_SUCCESS)
        // {
        //     LOG_FAIL("Load labels error");
        //     return STATUS_FAILED;
        // }
        
    }

    else if(this->framework == "tflite")
    {
        // this->tflite_runtime = new TfLiteRuntime();
        // // init model
        // std::string path_model = DIR_MODEL_OBJECTDETECION_TFLITE + this->objectTarget + ".tflite";
        // char path[100];
        // strcpy(path, path_model.c_str());
        // LOG_INFO_(path);
        // if(this->tflite_runtime->initTfLiteNetwork(path, deviceTarget) != STATUS_SUCCESS)
        // {
        //     LOG_FAIL("Init SNPE object detector network error");
        //     return STATUS_FAILED;
        // }
        // // load label
        // std::string path_label = DIR_LABEL_OBJECTDETECION_TFLITE + this->objectTarget + ".txt";
        // std::cout << "[INFO]- Snpe path label: " << path_label << std::endl;
        // if(loadLabel(path_label, labels) != STATUS_SUCCESS)
        // {
        //     LOG_FAIL("Load labels error");
        //     return STATUS_FAILED;
        // } 
    }
    LOG_SUCCESS("Init Object Detector successfully");
    return STATUS_SUCCESS;
}
int ObjectDetector::executeObjectDetector(const cv::Mat& img, std::vector<ObjectTrace>& objects, float thres_detect)
{

    objects.clear();
    if(!img.empty() && thres_detect >= 0.0 && thres_detect <= 1.0)
    {
        // execute snpe
        if(this->framework == "snpe")
        {
            if(this->snpe_detector->executeSnpeObjectDetector(img, this->labels, objects, thres_detect) != STATUS_SUCCESS)
            {
                LOG_FAIL("Execute snpe failed, Please check your modols");
                return STATUS_FAILED;
            }
        }
        else if(this->framework == "tflite")
        {
            // // if(this->tflite_detector->excuteTfLiteDetector(img, this->labels, objects, thres_detect) != 0)
            // if(this->tflite_runtime->executeTfLiteYolo(img, this->labels, objects, thres_detect) != STATUS_SUCCESS)
            // {
            //     LOG_FAIL("Execute tflite failed, Please check your modols");
            //     return STATUS_FAILED;
            // }

        }
        else if(this->framework == "ncnn")
        {
            // if(this->ncnn_detector->executeNcnnDetector(img, this->labels, objects, thres_detect) != STATUS_SUCCESS)
            // {
            //     LOG_FAIL("Execute ncnn failed, Please check your modols");
            //     return STATUS_FAILED;
            // }
        }
        else if(this->framework == "mnn")
        {
            // if(this->mnn_detector->detect(img, this->labels, objects, thres_detect) != STATUS_SUCCESS)
            // {
            //     LOG_FAIL("Execute mnn failed, Please check your modols");
            //     return STATUS_FAILED;
            // }
        }
    }
    else
    {
        LOG_FAIL("Execute AI fail, please check your input again !!!");
        return STATUS_FAILED;
    }
    return STATUS_SUCCESS;
}

// int ObjectDetector::initObjectAttributes(std::string framework, std::string objectTarget)
// {
//     this->framework = framework;
//     this->objectTarget = objectTarget;
    
//     this->tflite_runtime = new TfLiteRuntime();
//     // init model
//     std::string path_model = DIR_LABEL_OBJECTDETECION_TFLITE + this->objectTarget + ".tflite";
//     char path[100];
//     strcpy(path, path_model.c_str());
//     std::cout << "[INFO]- TFLite path model: " << path << std::endl;
//     if(this->tflite_runtime->initTfLiteNetwork(path, "CPU") != STATUS_SUCCESS)
//     {
//         printf("Init TFLite object detector network error\n");
//         return STATUS_FAILED;
//     }
//     return STATUS_SUCCESS;
// }

// int ObjectDetector::executeObjectAttributes(const cv::Mat& img, Attributes& attributes)
// {
//     if(this->tflite_runtime->executeTfLiteAttributes(img, attributes) != STATUS_SUCCESS)
//     {
//         LOG_FAIL("Execute tflite failed, Please check your model");
//         return STATUS_FAILED;
//     }
//     return STATUS_SUCCESS;
// }