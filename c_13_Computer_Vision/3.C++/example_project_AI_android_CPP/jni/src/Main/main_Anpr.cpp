#include <fstream>

#include "DecodeFrame.hpp"
#include "AnprRecognizer.hpp"

bool SetAdspLibraryPath(std::string nativeLibPath) {
    std::stringstream path;
    path << nativeLibPath << ";/system/lib/rfsa/adsp;/system/vendor/lib/rfsa/adsp;/dsp";
    return setenv("ADSP_LIBRARY_PATH", path.str().c_str(), 1 /*override*/) == 0;
}

void runAnpr()
{
    // open video test
    const char* video_file = (char*)"/data/local/Thinhdv/video/y2met.mp4";
    DecodeFrame decoder;
    decoder.open(video_file);
    
    // export env for DSP
    SetAdspLibraryPath("/data/local/tmp/snpe/dsp/lib");

    AnprRecognizer* anpr = new AnprRecognizer();

    int iInit = anpr->init(Nations::China);

    // settingVehicle setting;
    // setting.nation = Nations::VN;
    // setting.objectsTrack = {};
    // // int iInit = anpr->initVehicle(setting);
    // int iInit = anpr->initVehicle_lt(setting);

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
        if(iInit == STATUS_SUCCESS)
        {
            auto start = std::chrono::high_resolution_clock::now();    

            std::vector<PlateInfor> outData;
            anpr->trackAnpr_cn(img, img4k, outData);

            // std::vector<VehicleInfor> outData;
            // // anpr->trackVehicle(img, img4k, outData);
            // anpr->trackVehicle_lt(img, img4k, outData);

            auto end = std::chrono::high_resolution_clock::now();
            auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            cout << "Inferencing: " << time.count() << " ms" << " = " << 1000 / (time.count() + 1) << " FPS\n";

            std::cout << "outData.size() = " << outData.size() << std::endl;
            for(auto out : outData)
            {
                if(out.isNewEvent) {
                    cv::rectangle(img, out.rect, Scalar(0, 255, 0), 1, LINE_AA);
                    std::string text{std::to_string(out.track_id) + " - " + out.typePlate};
                    cv::putText(img, text, cv::Point(out.rect.x, out.rect.y), FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 255), 1);
                    cv::putText(img, out.license, cv::Point(out.rect.x, out.rect.y + out.rect.height), FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 255), 2);
                    std::cout << "license " << out.track_id << " = " << out.license << std::endl;

                    // cv::rectangle(img, out.rectVehicle, Scalar(0, 255, 0), 1, LINE_AA);
                    // std::string text{std::to_string(out.iTrackID) + " - " + out.typeVehicle};
                    // cv::putText(img, text, cv::Point(out.rectVehicle.x, out.rectVehicle.y), FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 255), 1);
                    // cv::putText(img, out.plateInf.license, cv::Point(out.plateInf.rect.x, out.plateInf.rect.y + out.plateInf.rect.height), FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 255), 2);
                    // std::cout << "license " << out.iTrackID << " = " << out.plateInf.license << std::endl;

                    // outStream << out.strLicense << std::endl;

                    // std::string path{std::to_string(n) + "_" + std::to_string(out.track_id)};
                    // cv::imwrite("/data/local/tmp/outputs_ITS/" + path + ".jpg", img);
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
    runAnpr();

    return 0;
}

