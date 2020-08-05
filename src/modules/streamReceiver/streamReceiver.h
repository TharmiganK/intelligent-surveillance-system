/**
    Intelligent Surveillance System
    @file streamReceiver.h
    @author Tharmigan Krishnananthalingam
*/

#ifndef _STREAMRECEIVER_H
#define _STREAMRECEIVER_H

#include "../frameQueue/frameQueue.h"
#include "../packetQueue/packetQueue.h"
#include "../videoStream/videoStream.h"
#include <fstream>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavformat/avio.h>
#include <libswscale/swscale.h>
}

using namespace std;

/**
    @class Class to receive the video packets from RTSP stream.
    @details StreamReceiver contains AVFormat context information about the 
    RTSP stream and video codec information from the stream. It can return
    video packets from stream and can share the codec information with the
    decoder.
*/
class StreamReceiver {

    public:

        /**
            @brief Get the next video packet from video stream.
            @return Video packet from the stream;
        */
        virtual AVPacket GetVideoPacket(VideoStream& videoStream);

    public:
        void operator()(VideoStream& videoStream);

};

#endif