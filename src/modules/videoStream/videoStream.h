/**
    Intelligent Surveillance System
    @file videoStream.h
    @author Tharmigan & Lavinan
*/

#ifndef _VIDEOSTREAM_H
#define _VIDEOSTREAM_H

#include "../frameQueue/frameQueue.h"
#include "../packetQueue/packetQueue.h"

/**
    @struct A video stream structure
    @details VideoStream contains all the necessary information of a particular stream along
    with PacketQueue to store the received packets and FrameQueue to store the decoded frames.
*/
struct VideoStream {

    int streamID; /*!< ID of the associated video stream */
    FrameQueue frameQueue; /*!< Queue to store the frames */
    PacketQueue packetQueue; /*!< Queue to store the packets */
    const char *streamURL; /*!< URL of the associated video stream */
    AVFormatContext* formatCtx; /*!< RTSP stream format context information */
    AVCodecContext* videoCodecCtx; /*!< Video codec context information */
    int videoStreamIndex; /*!< The index of the video stream in the RTSP stream */
    bool isOpen; /*!< Value to indicate whether the stream is opened or not */
    double videoFPS; /*!< Frame per second of the video stream */
    double videoBaseTime; /*!< Base time of the video stream */
    struct SwsContext *imgCvtCtx; /*!< Image convert context information (YUV -> BGR) */
    int width; /*!< Width of the decoded frame */
    int height; /*!< Height of the decoded frame */
    int bufferImgSize; /*!< Image buffer size from video codec */
    uint8_t *buffer; /*!< Buffer image used for YUV to BGR conversion */
    AVCodec* videoCodec; /*!< Video codec decoder */

    /**
        @brief Constructor of struct VideoStream.
        @param streamID ID of the associated camera stream.
        @param streamURL URL of the associated camera stream.
    */
    VideoStream(int streamID, const char *streamURL, int queueCapacity);

    /**
        @brief Open the RTSP stream.
        @return true if RTSP stream is opened sucessfully; false otherwise.
    */
    virtual bool OpenStream();

    /**
        @brief Close the RTSP stream.
        @return true if RTSP stream is closed sucessfully; false otherwise.
    */
    virtual bool CloseStream();

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