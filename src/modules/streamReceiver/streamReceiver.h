/**
    Intelligent Surveillance System
    @file streamReceiver.h
    @author Tharmigan Krishnananthalingam
*/

#ifndef _STREAMRECEIVER_H
#define _STREAMRECEIVER_H

#include "../frameManager/frameManager.h"
#include "../packetManager/packetManager.h"
#include "../decoder/decoder.h"
#include <fstream>
#include <thread>
#include <X11/Xlib.h>
#include <deque>

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

    private:

        AVFormatContext* formatCtx; /*!< RTSP stream format context information */
        AVCodecContext* videoCodecCtx; /*!< Video codec context information */
        int videoStreamIndex; /*!< The index of the video stream in the RTSP stream */
        bool isOpen; /*!< Value to indicate whether the stream is opened or not */
        double videoFPS; /*!< Frame per second of the video stream */
        double videoBaseTime; /*!< Base time of the video stream */

    public:

        /**
            @brief Constructor of class StreamReceiver.
            @details Constructor of StreamRecevier initiates the attributes with
            the following default values.
        */
        StreamReceiver() :
            videoBaseTime(0.0),  
            videoFPS(0.0),  
            isOpen(false),
            videoStreamIndex(-1), 
            videoCodecCtx(NULL),
            formatCtx(NULL){;}

        /**
            @brief Destructor of class StreamReceiver.
        */
        virtual ~StreamReceiver() {

            CloseStream();

        }

        /**
            @brief Open the RTSP stream.
            @param inputStream RTSP stream URL.
            @return true if RTSP stream is opened sucessfully; false otherwise.
        */
        virtual bool OpenStream(std::string& inputStream);

        /**
            @brief Close the RTSP stream.
            @return true if RTSP stream is closed sucessfully; false otherwise.
        */
        virtual bool CloseStream();

        /**
            @brief Get the next video packet from video stream.
            @return Video packet from the stream;
        */
        virtual AVPacket GetVideoPacket();


        /**
            @brief Get the video codec information.
            @return The video codec context information from the stream.
        */
        AVCodecContext* GetVideoCodecInfo(){

            return videoCodecCtx;

        }

    public:
        AVCodecContext *codec_ctx;
        void setContext(AVCodecContext *codec_ctx);
        void operator()(PacketManager *packetManager, FrameManager *frameManager);

    private:

        /**
            @brief Open the video stream from RTSP stream.
            @return true if video stream is opened sucessfully; false otherwise.
        */
        bool OpenVideoStream();

        /**
            @brief Close the video stream from RTSP stream.
        */
        void CloseVideoStream();
};

#endif