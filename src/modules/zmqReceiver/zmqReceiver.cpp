/**
    Intelligent Surveillance System
    @file zmqReceiver.cpp
    @author Tharmigan Krishnananthalingam & Sanoojan Baliah
*/

#include "zmqReceiver.h"

ZMQReceiver::ZMQReceiver(const char *streamURL):

    streamURL(streamURL)
    {
        zmq::context_t context_(1);
        zmq::socket_t subscriber_(context_, ZMQ_SUB);
        subscriber = &subscriber_;
        subscriber->connect(streamURL); 
        subscriber->setsockopt( ZMQ_SUBSCRIBE, "", 0);
    }

void ZMQReceiver::receiveData(){
    zmq::message_t message1;
    subscriber->recv(&message1);
    h = (char*) message1.data();

    zmq::message_t message2;
    subscriber->recv(&message2, ZMQ_RCVMORE);
    w = (char*) message2.data();

    zmq::message_t message3;
    subscriber->recv(&message3, ZMQ_RCVMORE);
    data = (char*) message3.data();

    zmq::message_t message;
    subscriber->recv(&message);
    char* image_data = (char*) message.data();
		
    frame = cv::Mat(frame_width, frame_height, CV_8UC3, image_data);
}
