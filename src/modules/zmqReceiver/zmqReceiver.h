/**
    Intelligent Surveillance System
    @file zmqStreamer.h
    @author Tharmigan Krishnananthalingam & Sanoojan Baliah
*/

#ifndef _ZMQRECEIVER_H
#define _ZMQRECEIVER_H

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
// #include "../videoStream/videoStream.h"

#include <zmq.hpp>

class ZMQReceiver {
    zmq::socket_t* subscriber;
    const char *streamURL;
    // VideoStream videoStream;
    const char* h;
    const char* w;
    const char* data;
    cv::Mat frame;
    int frame_width;
    int frame_height;

    ZMQReceiver(const char *streamURL);

    // void operator()(VideoStream& videoStream);

    void receiveData();
};

#endif