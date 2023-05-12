/******************************************************************************** 
 Module: SnpeRuntime.cpp
 Author: LE MANH THANG
 Created: 21/12/2020
 Description: 
********************************************************************************/

#include "SnpeCommLib.hpp"

zdl::DlSystem::Runtime_t checkRuntime(zdl::DlSystem::Runtime_t runtime)
{
    static zdl::DlSystem::Version_t Version = zdl::SNPE::SNPEFactory::getLibraryVersion();

    std::cout << "SNPE Version: " << Version.asString().c_str() << std::endl; //Print Version number

    if (!zdl::SNPE::SNPEFactory::isRuntimeAvailable(runtime))
    {
        std::cerr << "Selected runtime not present. Falling back to GPU." << std::endl;
        runtime = SNPE_FALLBACK;
    }
    else
    {
        printf("Target runtime was selected successfully\n");
    }
    return runtime;
}
std::unique_ptr<zdl::DlContainer::IDlContainer> loadContainerFromFile(std::string containerPath)
{
    std::unique_ptr<zdl::DlContainer::IDlContainer> container;
    container = zdl::DlContainer::IDlContainer::open(zdl::DlSystem::String(containerPath.c_str()));
    return container;
}
std::unique_ptr<zdl::SNPE::SNPE> setBuilderOptions(std::unique_ptr<zdl::DlContainer::IDlContainer>& container,
                                                   zdl::DlSystem::RuntimeList runtimeList,
                                                   zdl::DlSystem::StringList outputs)
{

    zdl::DlSystem::Runtime_t target_device =  SNPE_RUNTIME;
    std::unique_ptr<zdl::SNPE::SNPE> snpe;
    zdl::SNPE::SNPEBuilder snpeBuilder(container.get());

    snpe = snpeBuilder.setOutputLayers(outputs)
            .setRuntimeProcessorOrder(runtimeList)
            .build();
    return snpe;
}
std::unique_ptr<zdl::SNPE::SNPE> setBuilderSNPE(std::string containerPath, std::vector<std::string> outputLayers, zdl::DlSystem::Runtime_t target_device)
{
    // load model DLC file, which is built by SNPE.
    std::unique_ptr<zdl::DlContainer::IDlContainer> container;
    container = zdl::DlContainer::IDlContainer::open(zdl::DlSystem::String(containerPath.c_str()));
    if (container == nullptr)
    {
       std::cerr << "Error while opening the container file." << std::endl;
       return nullptr;
    }
    printf("Loaded dlc file successfully\n");

    // choose and check target runtime
    static zdl::DlSystem::Runtime_t runtime = checkRuntime(target_device);
    // building network
    zdl::DlSystem::StringList outputs;
    if(outputLayers.size() > 0)
    {
        for(int i = 0; i < outputLayers.size(); i++)
            outputs.append(outputLayers[i].c_str());
    }
    std::unique_ptr<zdl::SNPE::SNPE> snpe;
    zdl::SNPE::SNPEBuilder snpeBuilder(container.get());
    snpe = snpeBuilder.setOutputLayers(outputs)
            .setRuntimeProcessor(runtime)
            .setCPUFallbackMode(true)
            .build();
    return snpe;   
}

std::unique_ptr<zdl::DlSystem::ITensor> convertMat2BgrFloat(std::unique_ptr<zdl::SNPE::SNPE>& snpe,const cv::Mat& img)
{
    std::unique_ptr<zdl::DlSystem::ITensor> input;
    const auto &strList_opt = snpe->getInputTensorNames();
    if (!strList_opt) throw std::runtime_error("Error obtaining Input tensor names");
    const auto &strList = *strList_opt;
    // Make sure the network requires only a single input
    assert (strList.size() == 1);
    // Create an input tensor that is correctly sized to hold the input of the network. Dimensions that have no fixed size will be represented with a value of 0. /
    const auto &inputDims_opt = snpe->getInputDimensions(strList.at(0));
    const auto &inputShape = *inputDims_opt;
    /* Calculate the total number of elements that can be stored in the tensor so that we can check that the input contains the expected number of elements.
       With the input dimensions computed create a tensor to convey the input into the network. */
    input = zdl::SNPE::SNPEFactory::getTensorFactory().createTensor(inputShape);
   // printf("SNPE input shape: [%d, %d, %d, %d]\n", inputShape[0], inputShape[1], inputShape[2], inputShape[3]);  
    //Padding the input vector so as to make the size of the vector to equal to an integer multiple of the batch size
    
    // printf("Input network target: %d with batch %d, w %d, h %d, c %d\n", input->getSize(),inputShape[0], inputShape[1], inputShape[2], inputShape[3] );
    cv::Mat bgr;
    cv::cvtColor(img, bgr, cv::COLOR_RGB2BGR);
    cv::Mat bgr_resize;  
    cv::resize(bgr, bgr_resize, cv::Size((int)inputShape[1], (int)inputShape[2]));
    cv::Mat flat = bgr_resize.reshape(1, bgr_resize.total()*bgr_resize.channels());
    std::vector<unsigned char> inputVec = bgr_resize.isContinuous() ? flat : flat.clone();

    if (input->getSize() != inputVec.size()) {
        std::cerr << "Size of input does not match network.\n"
                  << "Expecting: " << input->getSize() << "\n"
                  << "Got: " << inputVec.size() << "\n";
        return nullptr;
    }
    std::vector<float> inputVecFloat;
    for (int i = 0; i < inputVec.size(); i++)
    {        
        float fData = (float)((inputVec[i] - 128.0f) / 128.0f);
        inputVecFloat.push_back(fData);
    }
    // Copy the oaded input file contents into the networks input tensor. SNPE's ITensor supports C++ STL functions like std::copy() /
    std::copy(inputVecFloat.begin(), inputVecFloat.end(), input->begin());
    return input;
}
