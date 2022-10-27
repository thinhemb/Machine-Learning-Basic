#include <fstream>

#include "DecodeFrame.hpp"
#include "Anpr.hpp"

bool SetAdspLibraryPath(std::string nativeLibPath) {
    std::stringstream path;
    path << nativeLibPath << ";/system/lib/rfsa/adsp;/system/vendor/lib/rfsa/adsp;/dsp";
    return setenv("ADSP_LIBRARY_PATH", path.str().c_str(), 1 /*override*/) == 0;
}

void runAnpr()
{
    // open video test
    const char* video_file = (char*)"/data/local/Thinhdv/video/Finger Tutting Hand Dance.mp4";
    DecodeFrame decoder;
    decoder.open(video_file);
    
    // export env for DSP
    SetAdspLibraryPath("/data/local/tmp/snpe/dsp/lib");

    ANPR* anpr = new ANPR();

    emNation_t typeNation = emNation_t::VietNam;
    int iInit = anpr->Init_ANPR(typeNation);

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

            std::vector<stPlateInfor_t> outDataPlate;
            anpr->Exe_ANPR(img, img4k, outDataPlate);

            auto end = std::chrono::high_resolution_clock::now();
            auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            cout << "Inferencing: " << time.count() << " ms" << " = " << 1000 / (time.count() + 1) << " FPS\n";

            std::cout << "outData.size() = " << outDataPlate.size() << std::endl;
            for(auto out : outDataPlate)
            {
                if(out.bEvent) {
                    // cv::rectangle(srcimg, out.rect, Scalar(0, 255, 0), 1, LINE_AA);
                    // std::string text{std::to_string(out.track_id) + " - " + out.typePlate};
                    // cv::putText(srcimg, text, cv::Point(out.rect.x, out.rect.y), FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 255), 1);
                    // cv::putText(srcimg, out.license, cv::Point(out.rect.x, out.rect.y + out.rect.height), FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 255), 2);
                    std::cout << "license " << out.iTrackID << " = " << out.strLicense << std::endl;
                    // outStream << out.strLicense << std::endl;

                    // std::string path{std::to_string(n) + "_" + std::to_string(out.track_id)};
                    // cv::imwrite("/data/local/tmp/outputs_verify/" + path + ".jpg", srcimg);
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

