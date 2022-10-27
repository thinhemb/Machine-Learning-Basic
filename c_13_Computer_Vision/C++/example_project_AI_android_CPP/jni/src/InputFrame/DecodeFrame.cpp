

#include "DecodeFrame.hpp"

DecodeFrame::DecodeFrame(/* args */)
{
}

DecodeFrame::~DecodeFrame()
{
    av_frame_free(&decframe);
    av_frame_free(&frame);
    avcodec_close(vstrm->codec);
    avformat_close_input(&inctx);
}
int DecodeFrame::open( const char* file_video)
{
    // initialize FFmpeg library
    av_register_all();
    //  av_log_set_level(AV_LOG_DEBUG);
    int ret;
    // open input file context
    ret = avformat_open_input(&inctx, file_video, nullptr, nullptr);
    if (ret < 0) {
        std::cerr << "fail to avforamt_open_input(\"" << file_video << "\"): ret=" << ret;
        return -1;
    }
    // retrive input stream information
    ret = avformat_find_stream_info(inctx, nullptr);
    if (ret < 0) {
        std::cerr << "fail to avformat_find_stream_info: ret=" << ret;
        return -1;
    }

    // find primary video stream
    ret = av_find_best_stream(inctx, AVMEDIA_TYPE_VIDEO, -1, -1, &vcodec, 0);
    if (ret < 0) {
        std::cerr << "fail to av_find_best_stream: ret=" << ret;
        return -1;
    }
    vstrm_idx = ret;
    vstrm = inctx->streams[vstrm_idx];

    // open video decoder context
    ret = avcodec_open2(vstrm->codec, vcodec, nullptr);
    if (ret < 0) {
        std::cerr << "fail to avcodec_open2: ret=" << ret;
        return -1;
    }

    // // print input video stream informataion
    // std::cout
    //     << "file_video: " << file_video << "\n"
    //     << "format: " << inctx->iformat->name << "\n"
    //     << "vcodec: " << vcodec->name << "\n"
    //     << "size:   " << vstrm->codec->width << 'x' << vstrm->codec->height << "\n"
    //     << "fps:    " << av_q2d(vstrm->codec->framerate) << " [fps]\n"
    //     << "length: " << av_rescale_q(vstrm->duration, vstrm->time_base, {1,1000}) / 1000. << " [sec]\n"
    //     << "pixfmt: " << av_get_pix_fmt_name(vstrm->codec->pix_fmt) << "\n"
    //     << "frame:  " << vstrm->nb_frames << "\n"
    //     << std::flush;

    // initialize sample scaler
    dst_width = vstrm->codec->width;
    dst_height = vstrm->codec->height;
    const AVPixelFormat dst_pix_fmt = AV_PIX_FMT_BGR24;
    swsctx = sws_getCachedContext(
        nullptr, vstrm->codec->width, vstrm->codec->height, vstrm->codec->pix_fmt,
        dst_width, dst_height, dst_pix_fmt, SWS_BICUBIC, nullptr, nullptr, nullptr);
    if (!swsctx) {
        std::cerr << "fail to sws_getCachedContext";
        return -1;
    }
    std::cout << "Frame: " << dst_width << 'x' << dst_height << ',' << av_get_pix_fmt_name(dst_pix_fmt) << std::endl;

    // allocate frame buffer for output
    frame = av_frame_alloc();
    framebuf = std::vector<uint8_t>(avpicture_get_size(dst_pix_fmt, dst_width, dst_height));
    avpicture_fill(reinterpret_cast<AVPicture*>(frame), framebuf.data(), dst_pix_fmt, dst_width, dst_height);

    return 0;
}
cv::Mat DecodeFrame::getFrame()
{
    cv::Mat img;
    // decoding loop
    decframe = av_frame_alloc();

    bool end_of_stream = false;
    int got_pic = 0;
    AVPacket pkt;
    if (!end_of_stream) 
    {
        // read packet from input file
        int ret = av_read_frame(inctx, &pkt);
        if (ret < 0 && ret != AVERROR_EOF) {
            std::cerr << "fail to av_read_frame: ret=" << ret;
            //return 2;
        }
        if (ret == 0 && pkt.stream_index != vstrm_idx)
            return cv::Mat{}; // return empty Mat
        end_of_stream = (ret == AVERROR_EOF);
    }
    if (end_of_stream) {
        // null packet for bumping process
        av_init_packet(&pkt);
        pkt.data = nullptr;
        pkt.size = 0;
    }
    // decode video frame
    avcodec_decode_video2(vstrm->codec, decframe, &got_pic, &pkt);
    if (!got_pic)
        return cv::Mat{}; // return empty Mat
    // convert frame to OpenCV matrix
    sws_scale(swsctx, decframe->data, decframe->linesize, 0, decframe->height, frame->data, frame->linesize);
    {
        cv::Mat image(dst_height, dst_width, CV_8UC3, framebuf.data(), frame->linesize[0]);

        if(!image.empty())
        {
            // TODO
            img = image.clone();
        }
    }
    ++nb_frames;
    av_free_packet(&pkt);
    return img;
}