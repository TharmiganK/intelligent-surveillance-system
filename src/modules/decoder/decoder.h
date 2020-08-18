/**
    Intelligent Surveillance System
    @file decoder.h
    @author Tharmigan Krishnananthalingam
*/

#ifndef _DECODER_H
#define _DECODER_H
#include "../frameQueue/frameQueue.h"
#include "../packetQueue/packetQueue.h"
#include "../videoStream/videoStream.h"
#include <fstream>
#include <boost/log/trivial.hpp>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavformat/avio.h>
#include <libswscale/swscale.h>
}

/**
    @class Class to decode video packets
    @details Decoder contains a video codec to decode AVPackets to AVFrames.
    Swsscale function is used to convert YUV to BGR. And OpenCV Mat function
    is used to convert BGR frame to image Matrix.
*/
class Decoder {

    public:

        /**
            @brief Method to Decode video packet into YUV frame.
            @return Decoded YUV frame.
            @param avpkt Video Packet from the stream.
        */
        AVFrame *DecodeVideo(VideoStream& videoStream, const AVPacket *avpkt);

        /**
            @brief Method to convert YUV frame to BGR frame.
            @return Decoded BGR frame.
            @param frameYUV Decoded YUV frame.
        */
        AVFrame *GetBGRFrame(VideoStream& videoStream, AVFrame *frameYUV);

        /**
            @brief Method to convert BGR frame to Opencv Mat image.
            @return OpenCV Mat image.
            @param frameBGR Decoded BGR frame.
        */
        cv::Mat GetImage(VideoStream& videoStream, AVFrame *frameBGR);

        /**
            @brief Method to run in Multi-thread.
            @param videoStream RTSP video stream;
        */
        void operator()(VideoStream videoStreams[], int numberOfStreams);

};

#endif