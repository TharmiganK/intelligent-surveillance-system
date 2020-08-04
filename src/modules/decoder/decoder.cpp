/**
    Intelligent Surveillance System
    @file decoder.h
    @author Tharmigan Krishnananthalingam
*/

#include "decoder.h"

void Decoder::operator()(FrameManager *frameManager, PacketManager *packetManager) {

    AVPacket packet;
    av_init_packet(&packet);
    AVFrame *frameYUV;
    AVFrame *frameBGR;
    cv::Mat image;

    while (true){

        if (!packetManager->queueIsEmpty()){

            packet = packetManager->dequeuePacket();

            if (&packet){

                frameYUV = DecodeVideo(&packet);
                frameBGR = GetBGRFrame(frameYUV);

                if(frameBGR){
                    image = GetImage(frameBGR);
                    frameManager->enqueueFrame(image.clone());
                }

            }

        }

    }
    
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
