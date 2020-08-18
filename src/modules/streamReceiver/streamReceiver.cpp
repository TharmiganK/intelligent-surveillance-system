/**
    Intelligent Surveillance System
    @file streamReceiver.cpp
    @author Tharmigan Krishnananthalingam
*/

#include "streamReceiver.h"

#define MAX_NUM_PACKETS 1000

/**
    @details operator method accepts the initiated video stream
    object and continues to read packets from the stream and 
    enque them in packet queue.
    This method is used to run in threads.
*/
void StreamReceiver::operator()(VideoStream videoStreams[],int numberOfStreams) {

    AVPacket packet;
    av_init_packet(&packet);
    int count = 0;

    while (true) {

        for (int i = 0; i < numberOfStreams; i++) {

            packet = GetVideoPacket(videoStreams[i]);

            videoStreams[i].packetQueue.enqueuePacket(*av_packet_clone(&packet));

            av_free_packet(&packet);
            av_init_packet(&packet);
            
        }

        count++;

    }

    av_free_packet(&packet);      

}


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