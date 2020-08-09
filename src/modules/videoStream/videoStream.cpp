/**
    Intelligent Surveillance System
    @file videoStream.cpp
    @author Tharmigan Krishnananthalingam & Lavinan Selvaratnam
*/

#include "videoStream.h"

/**
    @details Constructor of VideoStream to define the attributes of a camera stream with
    default values and to initialize queues for the stream with queueCapacity.
*/
VideoStream::VideoStream(int streamID, const char *streamURL, int queueCapacity):

    streamID(streamID),
    streamURL(streamURL),
    frameQueue(streamID,queueCapacity),
    packetQueue(streamID,queueCapacity),
    videoBaseTime(0.0),
    videoFPS(0.0),
    isOpen(false),
    videoStreamIndex(-1),
    videoCodecCtx(NULL),
    formatCtx(NULL)

    {

    }

/**
    @details OpenStream method opens the RTSP stream by accepting the RTSP
    URL and configuresfps and basetime in addition to format context of 
    the stream.
*/
bool VideoStream::OpenStream(){

    CloseStream();

    // Register all components
    av_register_all();
    avformat_network_init();

    // Open media stream.
    if (avformat_open_input(&formatCtx, streamURL, NULL, NULL) != 0){

        BOOST_LOG_TRIVIAL(error) << "COULD NOT OPEN THE RTSP STREAM.";
        CloseStream();
        return false;

    }

    // Get format info.
    if (avformat_find_stream_info(formatCtx, NULL) < 0){

        BOOST_LOG_TRIVIAL(error) << "COULD NOT FIND THE STREM INFO.";
        CloseStream();
        return false;

    }

    // open video stream.
    bool hasVideo = OpenVideoStream();

    if (!hasVideo){

        BOOST_LOG_TRIVIAL(error) << "COULD NOT FIND A VIDEO STREAM.";
        CloseStream();
        return false;

    }

    isOpen = true;

    // Get stream information.
    if (videoStreamIndex != -1){

        videoFPS            = av_q2d(formatCtx->streams[videoStreamIndex]->r_frame_rate);
        // Need for convert time to ffmpeg time.
        videoBaseTime       = av_q2d(formatCtx->streams[videoStreamIndex]->time_base); 

    }


    width = videoCodecCtx->width;
    height = videoCodecCtx->height;

    bufferImgSize = avpicture_get_size(AV_PIX_FMT_BGR24, videoCodecCtx->width, videoCodecCtx->height);
    buffer = (uint8_t*) av_mallocz(bufferImgSize);

    imgCvtCtx = sws_getContext(videoCodecCtx->width, videoCodecCtx->height, videoCodecCtx->pix_fmt, 
                                videoCodecCtx->width, videoCodecCtx->height, AV_PIX_FMT_BGR24,
                                SWS_BICUBIC, NULL, NULL, NULL);

    videoCodec = avcodec_find_decoder(videoCodecCtx->codec_id);

    if (videoCodec){

        avcodec_open2(videoCodecCtx, videoCodec, NULL);

    }else{

        BOOST_LOG_TRIVIAL(error) << "COULD NOT FIND THE SUITABLE DECODER.";
        return false;

    }

    BOOST_LOG_TRIVIAL(info) << "RTSP STREAM IS OPENED SUCCESSFULLY.";
    return true;

}

/**
    @details OpenStream method closes the RTSP stream and close the 
    format context for the stream if created.
*/
bool VideoStream::CloseStream(){

    isOpen = false;

    // Close video.
    CloseVideoStream();

    if (formatCtx){

        BOOST_LOG_TRIVIAL(info) << "RTSP STREAM IS CLOSED SUCCESSFULLY.";
        avformat_close_input(&formatCtx);
        formatCtx = NULL;

    }

    return true;

}

/**
    @details OpenStream method searches for video stream and configures
    the video stream index and obtains the video codec information from 
    the video stream and update the videoCodecCtx.
*/
bool VideoStream::OpenVideoStream(){

    bool res = false;

    if (formatCtx){

        videoStreamIndex = -1;

        for (unsigned int i = 0; i < formatCtx->nb_streams; i++){

            if (formatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO){

                videoStreamIndex = i;
                videoCodecCtx = formatCtx->streams[i]->codec;
                res = true;
                BOOST_LOG_TRIVIAL(info) << "VIDEO STREAM IS FOUNDED.";

                break;

            }
        }
    }

    return res;

}

/**
    @details CloseVideoStream closes the video codec context if created
    and removes the videoStreamIndex;
*/
void VideoStream::CloseVideoStream(){

    if (videoCodecCtx){

        BOOST_LOG_TRIVIAL(info) << "VIDEO CODEC IS CLOSED SUCCESSFULLY.";

        avcodec_close(videoCodecCtx);
        videoCodecCtx = NULL;
        videoStreamIndex = -1;

    }

}