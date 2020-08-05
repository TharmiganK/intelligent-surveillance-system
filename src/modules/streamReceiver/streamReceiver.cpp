/**
    Intelligent Surveillance System
    @file streamReceiver.cpp
    @author Tharmigan Krishnananthalingam
*/

#include "streamReceiver.h"

void StreamReceiver::operator()(VideoStream& videoStream) {

    AVPacket packet;
    av_init_packet(&packet);
    int count = 0;

    while (count < 1000){

        packet = GetVideoPacket(videoStream);

        videoStream.packetQueue.enqueuePacket(*av_packet_clone(&packet));

        av_free_packet(&packet);
        av_init_packet(&packet);

        count++;

    }

    av_free_packet(&packet);      

}

/**
    @details OpenStream method opens the RTSP stream by accepting the RTSP
    URL and configuresfps and basetime in addition to format context of 
    the stream.
*/

/**
    @details GetVideoPacket method returns the video packets from the
    RTSP stream.
*/
AVPacket StreamReceiver::GetVideoPacket(VideoStream& videoStream){

    AVPacket packet;
    packet = AVPacket();
 
    if (videoStream.videoStreamIndex != -1){

        if (videoStream.isOpen){

            //Read packet.
            while (av_read_frame(videoStream.formatCtx, &packet) >= 0){

                int64_t pts = 0;
                pts = (packet.dts != AV_NOPTS_VALUE) ? packet.dts : 0;

                if(packet.stream_index == videoStream.videoStreamIndex){

                    // Convert ffmpeg frame timestamp to real frame number.
                    int64_t numberFrame = (double)((int64_t)pts - 
                        videoStream.formatCtx->streams[videoStream.videoStreamIndex]->start_time) * 
                        videoStream.videoBaseTime * videoStream.videoFPS; 

                    break;

                } 
            }
        } 
    }

    return packet;

}