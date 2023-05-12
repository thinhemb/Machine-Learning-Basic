#include <fstream>

#include "DecodeFrame.hpp"
#include "BlowRedLight.hpp"

bool SetAdspLibraryPath(std::string nativeLibPath) {
    std::stringstream path;
    path << nativeLibPath << ";/system/lib/rfsa/adsp;/system/vendor/lib/rfsa/adsp;/dsp";
    return setenv("ADSP_LIBRARY_PATH", path.str().c_str(), 1 /*override*/) == 0;
}

void runBlowRedLight()
{
    // open video test
    const char* video_file = (char*)"/data/local/Thinhdv/video/Finger Tutting Hand Dance.mp4";
    DecodeFrame decoder;
    decoder.open(video_file);
    
    // export env for DSP
    SetAdspLibraryPath("/data/local/tmp/snpe/dsp/lib");

    BlowRedLight* blowRedLight = new BlowRedLight();

    int iInit = blowRedLight->BRL_Init();

    stSettingBlowRedLight_t stSetting;
    stSetting.strAllowedObjects = {"", ""};
    stSetting.strLabelsSet = {{"", ""}, {"", ""}};
    stSetting.ptRegionsSet = {{cv::Point(), cv::Point()}, {cv::Point(), cv::Point()}};
    stSetting.iAllowedLeft = 0;
    stSetting.iAllowedRight = 0;
    stSetting.iWidthFrame = 1920;
    stSetting.iHeightFrame = 1080;
    int iSet = blowRedLight->BRL_Set(stSetting);

    // std::string outputLog = "/data/bkavai/test/result_license.txt";
    // std::ofstream outStream(outputLog);
    // if(!outStream.is_open()) {
    //     std::cerr << "[ERROR] Failed to open stream for " << outputLog << "." << std::endl;
    // }

    int n = 1;
    while(1)
    {
        printf("--------------------------------------------%d\n", n);
        cv::Mat image = decoder.getFrame();
        // cv::Mat image = cv::imread("/data/local/tmp/images/bienMy01007858.jpg");
        if(image.empty()) {
            break;
        }
        cv::Mat img;
        cv::resize(image, img, cv::Size(1920, 1080));
        if((iInit == STATUS_SUCCESS) && (iSet == STATUS_SUCCESS))
        {
            auto start = std::chrono::high_resolution_clock::now();    

            std::vector<stOutputBlowRedLight_t> outData;
            blowRedLight->BRL_Exe(img, outData);

            auto end = std::chrono::high_resolution_clock::now();
            auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            cout << "Inferencing: " << time.count() << " ms" << " = " << 1000 / (time.count() + 1) << " FPS\n";

            std::cout << "outData.size() = " << outData.size() << std::endl;
            for(auto out : outData)
            {
                cv::rectangle(img, out.riBox, cv::Scalar(0, 0, 255), 1, cv::LINE_AA);
                std::string text{std::to_string(out.iTrackID) + " - " + out.strType};
                cv::putText(img, text, cv::Point(out.riBox.x, out.riBox.y), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 255), 1);

                // n += 1;
            }

            // cv::imwrite("/data/local/tmp/outputs/" + std::to_string(n) + ".jpg", srcimg);
            n += 1;
        }
    }
    // outStream.close();
}

int main()
{
    runBlowRedLight();

    return 0;
}

