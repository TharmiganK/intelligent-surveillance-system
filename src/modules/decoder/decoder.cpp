#include "decoder.h"

    Decoder::Decoder(AVCodecContext *codec_ctx) {
            this->Decoder::codec_ctx = codec_ctx;
    }
    

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
