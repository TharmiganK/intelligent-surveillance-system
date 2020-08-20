/**
    Intelligent Surveillance System
    @file packetQueue.h
    @author Lavinan Selvaratnam
*/

#ifndef _PACKETQUEUE_H
#define _PACKETQUEUE_H
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <boost/log/trivial.hpp>
#include <boost/circular_buffer.hpp>
#include <mutex>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavformat/avio.h>
#include <libswscale/swscale.h>
}

/**
    @class Class to manage the packet queue which contains packets which are the compressed frames
    @details PacketQueue has a thread safety queue to store the packets temporarily until
    it is accessed. When queue is full it drops the first packet before adding a new one.
*/
class PacketQueue {

    private:

        boost::circular_buffer<AVPacket> queue; /*!< A circular buffer queue to keep the incoming frames */
        int queueCapacity; /*!< Maximum number of frames that can be present inside the queue */
        std::mutex mutexForQueue; /*!< Mutex to give exclusive access to the queue */
        AVPacket currentPacket; /*!< Packet which is currently consumed from the queue to decode */
        
    public:

        /**
            @brief Constructor of class PacketQueue.
            @param streamID ID of the associated camera stream.
            @param queueCapacity Number of packets that can exist in the queue.
        */
        PacketQueue(int queueCapacity);

        /**
            @brief To add a packet at the end of queue.
            @param newFrame The packet that should be added.
        */
        bool enqueuePacket(AVPacket newPacket);


        /**
            @brief Take out the first packet from the queue.
            @return The packet which was taken from the queue.
        */
        AVPacket dequeuePacket();

        /**
            @brief Check the numer of packet present in the queue.
            @return number of packet that is present in the queue.
        */
        int queueLength();

        /**
            @brief Check whether there is any packet present in the queue.
            @return true if there is no packet and false if there is any packet persent.
        */
        bool queueIsEmpty();

    private:

        /**
            @brief Drop the first packet in the queue
        */
        bool dropPacket();

};

#endif