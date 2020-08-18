/**
    Intelligent Surveillance System
    @file main.cpp
*/

/**
    @todo Create makefile
    @todo Create configs class
    @todo unit test
 */

#include <iostream>
#include <opencv2/opencv.hpp>
#include <thread>
#include <X11/Xlib.h>
#include <boost/log/trivial.hpp>
#include "modules/streamReceiver/streamReceiver.h"
#include "modules/display/display.h"
#include "modules/decoder/decoder.h"
#include "modules/frameQueue/frameQueue.h"
#include "modules/packetQueue/packetQueue.h"
#include "modules/videoStream/videoStream.h"

#define STREAM_URL "rtsp://admin:admin@192.168.1.3:8554/live"
#define QUEUE_CAPACITY 30
#define STREAM_ID 1
#define NUMBER_OF_STREAMS 4

/**
    @brief Main function
    @details Temporary main function to instantiate a Video Stream, Stream Receiver, Decoder and a Display
    for a single stream and start receiving, decoding and displaying frames as seperate threads.
    Compressed frames (called as packets here) are received by Stream Receiver and stored in a Packet Queue. 
    Where decoder consumes the packets, decodes them and stores the decoded frames
    in a Frame Queue. Display consumes the decoded frames and displays them.
*/
int main() {

    BOOST_LOG_TRIVIAL(info) << "STARTING";
    //Creating an array of video streams.
    VideoStream* videoStreams = new VideoStream[NUMBER_OF_STREAMS];
    for (int i = 0; i < NUMBER_OF_STREAMS; i++) { 

        videoStreams[i].OpenStream(STREAM_URL);

    }

    BOOST_LOG_TRIVIAL(info) << "INITIALIZING";
    //Creating seperate threads to receive the frames, decode and display
    XInitThreads();
    std::thread streamReceiver1(StreamReceiver(),std::ref(videoStreams),NUMBER_OF_STREAMS);
    std::thread decoder1(Decoder(),std::ref(videoStreams),NUMBER_OF_STREAMS);
	std::thread display1(display(), std::ref(videoStreams),NUMBER_OF_STREAMS);

    BOOST_LOG_TRIVIAL(info) << "PROCESSING";
    
    //Waiting until the processes are over
    streamReceiver1.join();
    decoder1.join();
    display1.join();

    BOOST_LOG_TRIVIAL(info) << "PROCESS IS FINISHED";

    //Closing the video stream
    for (int i = 0; i < NUMBER_OF_STREAMS; i++) { 

        videoStreams[i].CloseStream();

    }
    
	return 0;

}