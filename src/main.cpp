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
//#include <X11/Xlib.h>
#include <boost/log/trivial.hpp>
#include <boost/asio.hpp>
#include "modules/streamReceiver/streamReceiver.h"
#include "modules/display/display.h"
#include "modules/decoder/decoder.h"
#include "modules/frameQueue/frameQueue.h"
#include "modules/packetQueue/packetQueue.h"
#include "modules/videoStream/videoStream.h"
#include "modules/processor/processor.h"
#include "modules/outputStreamer/outputStreamer.h"
#include "modules/httpServer/httpServer.h"
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

//#define STREAM_URL "rtsp://192.168.1.3:8080/h264_ulaw.sdp"
#define STREAM_URL "rtsp://admin:ELBXGJ@192.168.1.6:554/h264_stream"

#define STREAM_CAMERA 0
#define QUEUE_CAPACITY 500
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

    mongocxx::instance inst{};
    mongocxx::client conn{mongocxx::uri{}};
    auto collection = conn["video_analytics"]["camera_streams"];
    auto cursor = collection.find({});
    // bsoncxx::builder::stream::document document{};
    // document << "stream_id" << 4;
    // document << "stream_url" << "rtsp://wowzaec2demo.streamlock.net/vod/mp4:BigBuckBunny_115k.mov";
    // collection.insert_one(document.view());
    int i = 0;

    for (auto&& doc : cursor) {

        processors[i].addStream(new VideoStream(i, doc["stream_url"].get_utf8().value.to_string().c_str(), QUEUE_CAPACITY));
        processors[i].process();
        i++;

    }

    BOOST_LOG_TRIVIAL(info) << "PROCESSING";

    //Waiting until the processes are over
    for (int i = 0; i < NUMBER_OF_STREAMS; i++) {

        processors[i].join();

    }

    BOOST_LOG_TRIVIAL(info) << "PROCESS IS FINISHED";
    
    return 0;
//     unsigned short port = 1234;

//     try{
//         HttpServer http_server;

//         unsigned int thread_pool_size = std::thread::hardware_concurrency() * 2;

//         if(thread_pool_size == 0)
//             thread_pool_size = 2;

//         http_server.Start(port, thread_pool_size);

//         //keep alive server for 5 minutes, delete this and next line
//         //if you want to keep it alive for infinite time
//         std::this_thread::sleep_for(std::chrono::seconds(60 * 5));

//         http_server.Stop();

//     }catch(boost::system::system_error &e){
//         std::cout<<"Error occured, Error code = "<<e.code() 
//              <<" Message: "<<e.what();
//     }

//   return 0;

}