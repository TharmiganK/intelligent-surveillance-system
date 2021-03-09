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
//#include <X11/Xlib.h>
#include <boost/log/trivial.hpp>
#include "modules/streamReceiver/streamReceiver.h"
#include "modules/display/display.h"
#include "modules/decoder/decoder.h"
#include "modules/frameQueue/frameQueue.h"
#include "modules/packetQueue/packetQueue.h"
#include "modules/videoStream/videoStream.h"
#include "modules/outputStreamer/outputStreamer.h"

//#define STREAM_URL "rtsp://192.168.1.3:8080/h264_ulaw.sdp"
#define STREAM_URL "rtsp://admin:ELBXGJ@192.168.1.6:554/h264_stream"

#define STREAM_CAMERA 0
#define QUEUE_CAPACITY 500
#define STREAM_ID 1

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
    //Creating a sample VideoStream instance.
    VideoStream videoStream1(STREAM_ID, STREAM_URL, QUEUE_CAPACITY);
	//VideoStream videoStream2(2, "rtsp://192.168.1.3:8080/h264_ulaw.sdp", QUEUE_CAPACITY);
    videoStream1.OpenStream();

    BOOST_LOG_TRIVIAL(info) << "INITIALIZING";
    //Creating seperate threads to receive the frames, decode and display
    //XInitThreads();
    std::thread streamReceiver1(StreamReceiver(),std::ref(videoStream1));
    std::thread decoder1(Decoder(),std::ref(videoStream1));
	//std::thread display1(display(), std::ref(videoStream1.frameQueue));
	std::thread streamer1(Streamer(), std::ref(videoStream1));
  
    BOOST_LOG_TRIVIAL(info) << "PROCESSING";
    
    //Waiting until the processes are over
    streamReceiver1.join();
    decoder1.join();
    // display1.join();
    streamer1.join();

    BOOST_LOG_TRIVIAL(info) << "PROCESS IS FINISHED";

    //Closing the video stream
    videoStream1.CloseStream();
    
	return 0;

    //Main function
}