#include "yolov4_tiny.hpp"

static int num_line = 0;

static std::string read_file(const char* path, bool count_line)
{
    std::string result = "";
    std::ifstream f(path);
    if(!f.good()) 
        return result;
    else {
        std::string temp;
        while(std::getline(f, result)) {
            temp.append(result);
            if(count_line == true) {
                num_line ++;
            }
        }
        result = temp;
        f.close();
    }
    num_line = 0;
    return result;
}


NcnnDetect::NcnnDetect()
{
    this->net = new ncnn::Net();
    this->g_blob_pool_allocator_detect = new ncnn::UnlockedPoolAllocator();
    this->g_workspace_pool_allocator_detect = new ncnn::PoolAllocator();
}

NcnnDetect::~NcnnDetect()
{
    if(this->net)
        delete this->net;
    if(this->g_blob_pool_allocator_detect)
        delete this->g_blob_pool_allocator_detect;
    if(this->g_workspace_pool_allocator_detect)
        delete this->g_workspace_pool_allocator_detect;
}

int NcnnDetect::initNcnnDetect(const char* model_bin, const char* model_param)
{
    this->net->opt.num_threads = 4;
    this->net->opt.lightmode = true;
    this->net->opt.blob_allocator = this->g_blob_pool_allocator_detect;
    this->net->opt.workspace_allocator = this->g_workspace_pool_allocator_detect;

    // this->net->opt.lightmode = true;
    // this->net->opt.num_threads = 4; //You need to compile with libgomp for multi thread support
    // this->net->opt.blob_allocator = this->g_blob_pool_allocator_detect;
    // this->net->opt.workspace_allocator = this->g_workspace_pool_allocator_detect;
    // this->net->opt.use_vulkan_compute = true; //You need to compile with libvulkan for gpu support
    // this->net->opt.use_winograd_convolution = true;
    // this->net->opt.use_sgemm_convolution = true;
    // this->net->opt.use_fp16_packed = true;
    // this->net->opt.use_fp16_storage = true;
    // this->net->opt.use_fp16_arithmetic = true;
    // this->net->opt.use_packing_layout = true;
    // this->net->opt.use_shader_pack8 = false;
    // this->net->opt.use_image_storage = false;

    int ret = this->net->load_param(model_param);
    if(ret != 0) {
        std::cout << "load model param error!" << std::endl;
        return ret;
    }
    ret = this->net->load_model(model_bin);
    if(ret != 0) {
        std::cout << "load model bin error!" << std::endl;
        return ret;
    }
    std::string temp = read_file(model_param, false);
    // Đọc file model param
    if(!temp.empty()) {
        // lấy size input từ model param
        std::regex re("data 0=([0-9]+) 1=([0-9]+) 2=[0-9]{1}");
        std::smatch m; //smatch là một bản khởi tạo của mẫu lớp match_results cho các kết quả phù hợp trên các đối tượng chuỗi.
        std::regex_search(temp, m, re);
        if(!m.empty()) {
            for(auto i_m : m) {
                if(std::regex_match(i_m.str(), re)) {
                    try {
                        std::string s_w = i_m.str().substr(7, 9);
                        std::string s_h = i_m.str().substr(13, 15);
                        this->width_model = std::stoi(s_w);
                        this->height_model = std::stoi(s_h);
                        // size image(input)
                    } catch(const std::exception &e) {
                        this->width_model = 416;
                        this->height_model = 416;
                    }
                }
            }
        }
    }
    this->width_model = 416;
    this->height_model = 416;
    std::cout << "ncnn network input: " << this->width_model << "x" << this->height_model << std::endl;
    return 0;
}

int NcnnDetect::runNcnnDetect(const cv::Mat& img, std::vector<std::string> &labels, std::vector<TrackingBox> &objects, float threshold , int frame)
{
    int img_w = img.cols;
    int img_h = img.rows;
    //ncnn::Mat::from_pixels_resize cấu trúc thuận tiện từ dữ liệu pixel và thay đổi kích thước thành kích thước cụ thể
    ncnn::Mat in = ncnn::Mat::from_pixels_resize(img.data, ncnn::Mat::PIXEL_BGR2RGB, img.cols, img.rows, this->width_model, this->height_model);
    const float mean_vals[3] = {0, 0, 0};
    const float norm_vals[3] = {1/255.f, 1/255.f, 1/255.f};
    in.substract_mean_normalize(mean_vals, norm_vals);
    // substract_mean_normalize rút ngắn giá trị trung bình theo kênh, sau đó nhân với giá trị chuẩn hóa, chuyển 0 để bỏ qua
    ncnn::Extractor ex = this->net->create_extractor();
    
    ex.input("data", in);
    ncnn::Mat out;
    ex.extract("output", out, 0);
    //lấy kết quả theo tên blob trả về 0 nếu thành công

    objects.clear();
    for(int i=0; i<out.h; i++) {
        const float* values = out.row(i);
        try {
            if(values[1] >= threshold) {
                TrackingBox object;
                object.frame=frame;
                object.id=-1;
                object.label = labels[(int)values[0] ];
                object.obj_id = (int)values[0] - 1;
                object.score = values[1];
                object.box.x = (values[2] < 0) ? 0 : (values[2] * img_w);
                object.box.y = (values[3] < 0) ? 0 : (values[3] * img_h);
                object.box.width = values[4] * img_w - object.box.x;
                if(object.box.width + object.box.x > img_w)
                    object.box.width = img_w - object.box.x;
                object.box.height = values[5] * img_h - object.box.y;
                if(object.box.height + object.box.y > img_h)
                    object.box.height = img_h - object.box.y;
                objects.push_back(object);
            }
        } catch(const std::exception &e) {
            objects.clear();
            return -1;
        }
    }
    return 0;
}


