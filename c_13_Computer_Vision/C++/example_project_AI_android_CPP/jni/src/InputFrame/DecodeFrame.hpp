/*
    Module: DecodeFrame.cpp
    Author: ThangLmb
    Date: June 24, 2021
*/
#ifndef DecodeFrame_hpp
#define DecodeFrame_hpp


#include <iostream>
#include <unistd.h>
#include <iostream>
#include <vector>
// FFmpeg
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/pixdesc.h>
#include <libswscale/swscale.h>
}
// OpenCV
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
class DecodeFrame
{
private:
    /* data */
    AVFrame* decframe = nullptr;
    AVFrame* frame = nullptr;
    AVStream* vstrm = nullptr;
    AVFormatContext* inctx = nullptr;
    SwsContext* swsctx = nullptr;
    AVCodec* vcodec = nullptr;
    int vstrm_idx;
    int dst_width ;
    int dst_height;
    std::vector<uint8_t> framebuf;
    unsigned long nb_frames = 0;
public:
    DecodeFrame(/* args */);
    ~DecodeFrame();
    int open( const char* file_video);
    cv::Mat getFrame();
};




#endif