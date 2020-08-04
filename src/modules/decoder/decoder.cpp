/**
    Intelligent Surveillance System
    @file decoder.h
    @author Tharmigan Krishnananthalingam
*/

#include "decoder.h"

// Decoder::Decoder(AVCodecContext *codec_context) {

//     codec_ctx = codec_context;
//     width = codec_ctx->width;
//     height = codec_ctx->height;
//     bufferImgSize = avpicture_get_size(AV_PIX_FMT_BGR24, codec_ctx->width, codec_ctx->height);
//     buffer = (uint8_t*) av_mallocz(bufferImgSize);
//     imgCvtCtx = sws_getContext(codec_ctx->width, Decoder::codec_ctx->height,
//                     Decoder::codec_ctx->pix_fmt, Decoder::codec_ctx->width, Decoder::codec_ctx->height, AV_PIX_FMT_BGR24,
//                     SWS_BICUBIC, NULL, NULL, NULL);
 
// }
    

void Decoder::operator()(FrameManager *frameManager, PacketManager *packetManager) {
                    int size = avpicture_get_size(AV_PIX_FMT_YUV420P, Decoder::codec_ctx->width,
                    Decoder::codec_ctx->height);
                    uint8_t* picture_buffer = (uint8_t*) (av_malloc(size));
                    AVFrame* picture = av_frame_alloc();
                    AVFrame* picture_rgb = av_frame_alloc();
                    int size2 = avpicture_get_size(AV_PIX_FMT_RGB24, Decoder::codec_ctx->width,
                    Decoder::codec_ctx->height);
                        uint8_t* picture_buffer_2 = (uint8_t*) (av_malloc(size2));
                        avpicture_fill((AVPicture *) picture, picture_buffer, AV_PIX_FMT_YUV420P,
                    Decoder::codec_ctx->width, Decoder::codec_ctx->height);
                        avpicture_fill((AVPicture *) picture_rgb, picture_buffer_2, AV_PIX_FMT_RGB24,
                    Decoder::codec_ctx->width, Decoder::codec_ctx->height);
                    SwsContext *img_convert_ctx;
                    img_convert_ctx = sws_getContext(Decoder::codec_ctx->width, Decoder::codec_ctx->height,
                    Decoder::codec_ctx->pix_fmt, Decoder::codec_ctx->width, Decoder::codec_ctx->height, AV_PIX_FMT_BGR24,
                    SWS_BICUBIC, NULL, NULL, NULL);
                    int check = 0;
                    int cu = 0;
                    AVPacket dpacket;
                    av_init_packet(&dpacket);
                    while(true) {
                        if (!packetManager->queueIsEmpty()) {

                    dpacket = packetManager->dequeuePacket();
                    int result = avcodec_decode_video2(Decoder::codec_ctx, picture, &check, &dpacket);
                    std::cout << "Decoded frame: " << cu << std::endl;
                    sws_scale(img_convert_ctx, picture->data, picture->linesize, 0,
                                Decoder::codec_ctx->height, picture_rgb->data, picture_rgb->linesize);
                    cv::Mat image(Decoder::codec_ctx->height, Decoder::codec_ctx->width, CV_8UC3, picture_rgb->data[0], picture_rgb->linesize[0]);
                    // std::lock_guard<std::mutex> guard(myMutex);
                    frameManager->enqueueFrame(image.clone());
                        // av_free_packet(&dpacket);
                        // av_init_packet(&dpacket);
                    cu = cu + 1;
                        }
                    }
                        av_free(picture);
                        av_free(picture_rgb);
                        av_free(picture_buffer);
                        av_free(picture_buffer_2);
}

/**
    @details DecodeVideo method accepts the video packets from stream and decode
    it to a YUV video frame.
*/
AVFrame* Decoder::DecodeVideo(const AVPacket* avpkt){

    AVFrame *outFrameYUV = av_frame_alloc();

    if (codec_ctx){

        if (avpkt && outFrameYUV){

            int got_picture_ptr = 0;
            int videoFrameBytes = avcodec_decode_video2(codec_ctx, outFrameYUV, &got_picture_ptr, avpkt);

        }
    }

    return outFrameYUV;

}

/**
    @details GetBGRFrame method accepts the YUV formatted decoded video frame and 
    converts it to a BGR formatted video frame.
*/
AVFrame* Decoder::GetBGRFrame(AVFrame *frameYUV){

    AVFrame * frame = NULL;
    frame = av_frame_alloc();
    
    if (frame){

        avpicture_fill((AVPicture*) frame, buffer, AV_PIX_FMT_BGR24, width, height);

        sws_scale(imgCvtCtx, frameYUV->data, frameYUV->linesize, 0, height, frame->data, frame->linesize);
        
    }

    return (AVFrame *) frame;

}

/**
    @details GetImage method accepts the BGR formatted video frame and converts
    it to a OpenCV Mat image.
*/
cv::Mat Decoder::GetImage(AVFrame *frameBGR){

    cv::Mat image(codec_ctx->height, codec_ctx->width, CV_8UC3, frameBGR->data[0], frameBGR->linesize[0]);

    return image;

}
