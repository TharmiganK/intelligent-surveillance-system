/**
    Intelligent Surveillance System
    @file decoder.h
    @author Tharmigan Krishnananthalingam
*/

#include "decoder.h"

void Decoder::operator()(VideoStream& videoStream) {

    AVPacket packet;
    av_init_packet(&packet);
    AVFrame *frameYUV;
    AVFrame *frameBGR;
    cv::Mat image;
    int count = 0;

    while (count < 1000){

        if (!videoStream.packetQueue.queueIsEmpty()){

            packet = videoStream.packetQueue.dequeuePacket();

            if (&packet){

                frameYUV = DecodeVideo(videoStream, &packet);
                frameBGR = GetBGRFrame(videoStream, frameYUV);

                if(frameBGR){

                    image = GetImage(videoStream, frameBGR);
                    videoStream.frameQueue.enqueueFrame(image.clone());

                }

                count++;
                BOOST_LOG_TRIVIAL(info) << "Decoded frame : " << count << " from stream ID : " << videoStream.streamID;

            }

        }

    }
    
}

/**
    @details DecodeVideo method accepts the video packets from stream and decode
    it to a YUV video frame.
*/
AVFrame* Decoder::DecodeVideo(VideoStream& videoStream, const AVPacket* avpkt){

    AVFrame *outFrameYUV = av_frame_alloc();

    if (videoStream.videoCodecCtx){

        if (avpkt && outFrameYUV){

            int got_picture_ptr = 0;
            int videoFrameBytes = avcodec_decode_video2(videoStream.videoCodecCtx, outFrameYUV, &got_picture_ptr, avpkt);

        }
    }

    return outFrameYUV;

}

/**
    @details GetBGRFrame method accepts the YUV formatted decoded video frame and 
    converts it to a BGR formatted video frame.
*/
AVFrame* Decoder::GetBGRFrame(VideoStream& videoStream, AVFrame *frameYUV){

    AVFrame * frame = NULL;
    frame = av_frame_alloc();
    
    if (frame){

        avpicture_fill((AVPicture*) frame, videoStream.buffer, AV_PIX_FMT_BGR24, videoStream.width, videoStream.height);

        sws_scale(videoStream.imgCvtCtx, frameYUV->data, frameYUV->linesize, 0, videoStream.height, frame->data, frame->linesize);
        
    }

    return (AVFrame *) frame;

}

/**
    @details GetImage method accepts the BGR formatted video frame and converts
    it to a OpenCV Mat image.
*/
cv::Mat Decoder::GetImage(VideoStream& videoStream, AVFrame *frameBGR){

    cv::Mat image(videoStream.height, videoStream.width, CV_8UC3, frameBGR->data[0], frameBGR->linesize[0]);

    return image;

}
