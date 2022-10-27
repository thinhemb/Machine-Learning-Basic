#include <fstream>

#include "DecodeFrame.hpp"
#include "Vehicle.hpp"

bool SetAdspLibraryPath(std::string nativeLibPath) {
    std::stringstream path;
    path << nativeLibPath << ";/system/lib/rfsa/adsp;/system/vendor/lib/rfsa/adsp;/dsp";
    return setenv("ADSP_LIBRARY_PATH", path.str().c_str(), 1 /*override*/) == 0;
}

void runVHC()
{
    // open video test
    const char* video_file = (char*)"/data/local/Thinhdv/video/Finger Tutting Hand Dance.mp4";
    DecodeFrame decoder;
    decoder.open(video_file);
    
    // export env for DSP
    SetAdspLibraryPath("/data/local/tmp/snpe/dsp/lib");

    Vehicle* vehicle = new Vehicle();

    stSettingVehicle_t stSettingVehicle;
    stSettingVehicle.emTypeNation = emNation_t::VietNam;
    stSettingVehicle.strVehicles = {};
    int iInit = vehicle->VHC_Init(stSettingVehicle);

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
        cv::resize(image, img4k, cv::Size(3840, 2160));

        cv::Rect2f rfBox = cv::Rect2f(cv::Point2f(img.cols * 0.2f, img.rows * 0.2f), cv::Point2f(img.cols * 0.8f, img.rows * 0.8f));
        cv::rectangle(img, rfBox, cv::Scalar(0, 255, 0), 1, cv::LINE_AA);

        if(iInit == STATUS_SUCCESS)
        {
            auto start = std::chrono::high_resolution_clock::now();    

            std::vector<stVehicleInfor_t> outDataVehicle;
            vehicle->VHC_Exe(img, img4k, outDataVehicle);

            auto end = std::chrono::high_resolution_clock::now();
            auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            cout << "Inferencing: " << time.count() << " ms" << " = " << 1000 / (time.count() + 1) << " FPS\n";

            std::cout << "outData.size() = " << outDataVehicle.size() << std::endl;
            for(auto out : outDataVehicle)
            {
                cv::rectangle(img, out.riBoxVehicle, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);
                cv::putText(img, out.strTypeVehicle, out.riBoxVehicle.br(), FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 255), 1);
                if(out.bEvent) {
                    cv::rectangle(img, out.riBoxVehicle, cv::Scalar(0, 255, 0), 1, cv::LINE_AA);
                    cv::rectangle(img, out.stPlateInfor.riBoxPlate, cv::Scalar(0, 255, 0), 1, cv::LINE_AA);
                    std::string text{std::to_string(out.iTrackID) + " - " + out.strTypeVehicle};
                    cv::putText(img, text, cv::Point(out.riBoxVehicle.x, out.riBoxVehicle.y), FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 255), 1);
                    cv::putText(img, out.stPlateInfor.strLicense, 
                                cv::Point(out.stPlateInfor.riBoxPlate.x, out.stPlateInfor.riBoxPlate.y + out.stPlateInfor.riBoxPlate.height), FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 255), 2);
                    std::cout << "license " << out.iTrackID << " = " << out.stPlateInfor.strLicense << std::endl;
                    // outStream << out.strLicense << std::endl;

                    std::string path{std::to_string(n) + "_" + std::to_string(out.iTrackID)};
                    cv::imwrite("/data/local/tmp/outputs_VHC/" + path + ".jpg", img);
                }
                // n += 1;
            }

            // cv::imwrite("/data/local/tmp/outputs/" + std::to_string(n) + ".jpg", srcimg);
            n += 1;
        }
    }
    // outStream.close();
}

int main(int argc, char** argv)
{
    runVHC();

    return 0;
}
