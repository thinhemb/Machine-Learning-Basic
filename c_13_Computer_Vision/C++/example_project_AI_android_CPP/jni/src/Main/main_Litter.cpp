#include <fstream>

#include "DecodeFrame.hpp"
#include "Litter.hpp"

bool SetAdspLibraryPath(std::string nativeLibPath) {
    std::stringstream path;
    path << nativeLibPath << ";/system/lib/rfsa/adsp;/system/vendor/lib/rfsa/adsp;/dsp";
    return setenv("ADSP_LIBRARY_PATH", path.str().c_str(), 1 /*override*/) == 0;
}

void runLT()
{
    // open video test
    const char* video_file = (char*)"/data/local/Thinhdv/video/Finger Tutting Hand Dance.mp4";
    DecodeFrame decoder;
    decoder.open(video_file);
    
    // export env for DSP
    SetAdspLibraryPath("/data/local/tmp/snpe/dsp/lib");

    Litter* litter = new Litter();
    int iInit = litter->LT_Init();

    stSettingLitter_t stSettingLitter;
    stSettingLitter.ptRegionSet = {};
    stSettingLitter.strObjectSet = {};
    int iSet = litter->LT_Set(stSettingLitter);

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
        cv::Mat img, img4k;
        cv::resize(image, img, cv::Size(1920, 1080));
        // cv::resize(image, img4k, cv::Size(3840, 2160));

        // cv::Rect2f rfBox = cv::Rect2f(cv::Point2f(img.cols * 0.2f, img.rows * 0.2f), cv::Point2f(img.cols * 0.8f, img.rows * 0.8f));
        // cv::rectangle(img, rfBox, cv::Scalar(0, 255, 0), 1, cv::LINE_AA);

        if(iInit == 0 && iSet == 0)
        {
            auto start = std::chrono::high_resolution_clock::now();

            std::vector<stOutputLitter_t> outData;
            litter->LT_Exe(img, outData);

            auto end = std::chrono::high_resolution_clock::now();
            auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            cout << "Inferencing: " << time.count() << " ms" << " = " << 1000 / (time.count() + 1) << " FPS\n";

            std::cout << "outData.size() = " << outData.size() << std::endl;
            for(auto out : outData)
            {
                cv::rectangle(img, out.riBox, cv::Scalar(0, 0, 255), 1, cv::LINE_AA);
                cv::putText(img, out.strType, out.riBox.tl(), FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 255), 1);

                cv::rectangle(img, out.stTrash.riBox, cv::Scalar(0, 0, 255), 1, cv::LINE_AA);
                std::string text{std::to_string(out.stTrash.iTrackID) + " - " + out.stTrash.strType};
                cv::putText(img, text, out.stTrash.riBox.tl(), FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 255), 1);
                std::cout << TagLitter << " Event: " << text << std::endl;

                std::string path{std::to_string(n) + "_" + std::to_string(out.stTrash.iTrackID)};
                cv::imwrite("/data/local/tmp/outputs_LT/17/" + path + ".jpg", img);
                // n += 1;
            }

            // cv::imwrite("/data/local/tmp/outputs_LT/" + std::to_string(n) + ".jpg", img);
            n += 1;
        }
    }
    // outStream.close();
}

int main()
{
    runLT();

    return 0;
}