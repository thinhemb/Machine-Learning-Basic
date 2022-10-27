#ifndef ocr_db_crnn_hpp
#define ocr_db_crnn_hpp
#include "paddle_api.h" // NOLINT
#include <chrono>
#include "crnn_process.hpp"
#include "db_post_process.hpp"


using namespace paddle::lite_api; // NOLINT
using namespace std;


void NeonMeanScale(const float *din, float *dout, int size,
                   const std::vector<float> mean,
                   const std::vector<float> scale);
cv::Mat DetResizeImg(const cv::Mat img, int max_size_len,
                     std::vector<float> &ratio_hw);
void RunRecModel_withCLS(std::vector<std::vector<std::vector<int>>> boxes, cv::Mat img,
                 std::shared_ptr<PaddlePredictor> predictor_crnn,
                 std::shared_ptr<PaddlePredictor> predictor_cls,
                 std::vector<std::string> &rec_text,
                 std::vector<float> &rec_text_score,
                 std::vector<std::string> charactor_dict,
                 std::map<std::string, double> Config);

void RunRecModel(std::vector<std::vector<std::vector<int>>> boxes, cv::Mat img,
                 std::shared_ptr<PaddlePredictor> predictor_crnn,
                 std::vector<std::string> &rec_text,
                 std::vector<float> &rec_text_score,
                 std::vector<std::string> charactor_dict,
                 std::map<std::string, double> Config);
std::vector<std::vector<std::vector<int>>>
RunDetModel(std::shared_ptr<PaddlePredictor> predictor, cv::Mat img,
            std::map<std::string, double> Config);
std::shared_ptr<PaddlePredictor> loadModel(std::string model_file);
cv::Mat Visualization(cv::Mat srcimg,
                      std::vector<std::vector<std::vector<int>>> boxes);
std::map<std::string, double> LoadConfigTxt(std::string config_path) ;


#endif