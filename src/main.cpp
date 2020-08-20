/**
    Intelligent Surveillance System
    @file main.cpp
*/

/**
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
#include "modules/processor/processor.h"
#include "modules/outputStreamer/outputStreamer.h"


#define STREAM_URL "rtsp://wowzaec2demo.streamlock.net/vod/mp4:BigBuckBunny_115k.mov"
#define QUEUE_CAPACITY 30
#define STREAM_ID 1
#define NUMBER_OF_STREAMS 3
#define NUMBER_OF_WORKERS 3

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
    //Creating processors
    Processor* processors = new Processor[NUMBER_OF_STREAMS];

    //Initializing video streams and start processing
    BOOST_LOG_TRIVIAL(info) << "INITIALIZING";
    for (int i = 0; i < NUMBER_OF_STREAMS; i++) {

        processors[i].addStream(new VideoStream(i, STREAM_URL, QUEUE_CAPACITY));
        processors[i].process();

    }

    BOOST_LOG_TRIVIAL(info) << "PROCESSING";

    //Waiting until the processes are over
    for (int i = 0; i < NUMBER_OF_STREAMS; i++) {

        processors[i].join();

    }

    BOOST_LOG_TRIVIAL(info) << "PROCESS IS FINISHED";
    
	return 0;

}