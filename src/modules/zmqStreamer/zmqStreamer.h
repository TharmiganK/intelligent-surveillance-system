/**
    Intelligent Surveillance System
    @file zmqStreamer.h
    @author Tharmigan Krishnananthalingam & Sanoojan Baliah
*/


#ifndef _ZMQSTREAMER_H
#define _ZMQSTREAMER_H

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
// #include "../videoStream/videoStream.h"

#include <zmq.hpp>

/**
    @class Class to stream video with data
    @details ZMQStreamer contains a zeromq publisher.
    The publisher can publish data along with video through tcp link.
*/

class ZMQStreamer {
    zmq::socket_t* publisher;
    const char *streamURL;
    // VideoStream videoStream;
    const char* h;
    const char* w;
    const char* data;
    cv::Mat frame;
    int frame_size;

    ZMQStreamer(const char *streamURL);

    // void operator()(VideoStream& videoStream);

    void streamData();
};

#endif