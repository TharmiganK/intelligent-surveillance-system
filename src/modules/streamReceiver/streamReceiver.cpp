/**
    Intelligent Surveillance System
    @file streamReceiver.cpp
    @author Tharmigan Krishnananthalingam
*/

#include "streamReceiver.h"

void StreamReceiver::operator()(PacketManager *packetManager, FrameManager *frameManager) {

    OpenStream(stream_url);

    AVPacket packet;
    av_init_packet(&packet);

    std::thread d1(Decoder(videoCodecCtx),frameManager,packetManager);
    d1.join();
    
    int cnt = 0;

    while (cnt < 1000){

        packet = GetVideoPacket();

        packetManager->enqueuePacket(*av_packet_clone(&packet));

        av_free_packet(&packet);
        av_init_packet(&packet);

        cnt ++;

    }

    av_free_packet(&packet);
    CloseStream();        

}

/**
    @details OpenStream method opens the RTSP stream by accepting the RTSP
    URL and configuresfps and basetime in addition to format context of 
    the stream.
*/
bool StreamReceiver::OpenStream (std::string& inputStream){

    stream_url = inputStream;

    CloseStream();

    // Register all components
    av_register_all();
    avformat_network_init();

    // Open media stream.
    if (avformat_open_input(&formatCtx, inputStream.c_str(), NULL, NULL) != 0){

        CloseStream();
        return false;

    }

    // Get format info.
    if (avformat_find_stream_info(formatCtx, NULL) < 0){

        CloseStream();
        return false;

    }

    // open video stream.
    bool hasVideo = OpenVideoStream();

    if (!hasVideo){

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

    return true;

}

/**
    @details OpenStream method closes the RTSP stream and close the 
    format context for the stream if created.
*/
bool StreamReceiver::CloseStream(){

    isOpen = false;

    // Close video.
    CloseVideoStream();

    if (formatCtx){

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
bool StreamReceiver::OpenVideoStream(){

    bool res = false;

    if (formatCtx){

        videoStreamIndex = -1;

        for (unsigned int i = 0; i < formatCtx->nb_streams; i++){

            if (formatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO){

                videoStreamIndex = i;
                videoCodecCtx = formatCtx->streams[i]->codec;
                res = true;

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
void StreamReceiver::CloseVideoStream(){

    if (videoCodecCtx){

        avcodec_close(videoCodecCtx);
        videoCodecCtx = NULL;
        videoStreamIndex = -1;

    }

}

/**
    @details GetVideoPacket method returns the video packets from the
    RTSP stream.
*/
AVPacket StreamReceiver::GetVideoPacket(){

    AVPacket packet;
    packet = AVPacket();
 
    if (videoStreamIndex != -1){

        if (isOpen){

            //Read packet.
            while (av_read_frame(formatCtx, &packet) >= 0){

                int64_t pts = 0;
                pts = (packet.dts != AV_NOPTS_VALUE) ? packet.dts : 0;

                if(packet.stream_index == videoStreamIndex){

                    // Convert ffmpeg frame timestamp to real frame number.
                    int64_t numberFrame = (double)((int64_t)pts - 
                        formatCtx->streams[videoStreamIndex]->start_time) * 
                        videoBaseTime * videoFPS; 

                    break;

                } 
            }
        } 
    }

    return packet;

}