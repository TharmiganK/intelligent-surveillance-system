/**
    Intelligent Surveillance System
    @file zmqStreamer.cpp
    @author Tharmigan Krishnananthalingam & Sanoojan Baliah
*/

#include "zmqStreamer.h"

ZMQStreamer::ZMQStreamer(const char *streamURL):

    streamURL(streamURL)
    {
        zmq::context_t context_(1);
        zmq::socket_t publisher_(context_, ZMQ_PUB);
        publisher = &publisher_;
        publisher->bind(streamURL); 
    }

void ZMQStreamer::streamData(){
    publisher->send(h, strlen(h)+1, ZMQ_SNDMORE);
    publisher->send(w, strlen(w)+1, ZMQ_SNDMORE);
    publisher->send(data, strlen(data)+1, ZMQ_SNDMORE);
    publisher->send(frame.data, frame_size, ZMQ_NOBLOCK);
}

// int height = frame.rows;
// int width = frame.cols;
// int frame_size = 3*height*width*sizeof(uint8_t);
// std::string h_s = std::to_string(height);
// std::string w_s = std::to_string(width);
// const char* h = h_s.c_str();
// const char* w = w_s.c_str();
// std::string data_s = "Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!";
// const char* data = data_s.c_str(); 
