/**
   Intelligent Surveillance System
   @file packetManager.h
   @author Lavinan Selvaratnam
*/

#ifndef _PACKETMANAGER_H
#define _PACKETMANAGER_H
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <boost/log/trivial.hpp>
#include <deque>
#include <mutex>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavformat/avio.h>
#include <libswscale/swscale.h>
}

/**
    @class Class to manage the packet queue which contains packets which are the compressed frames
    @details PacketManager has a queue to store the packets temporarily until
    it is accessed. When queue is full it drops the first packet before adding a new one.
*/
class PacketManager {

    private:

        int streamID; /*!< ID of the associated video stream */
        std::deque<AVPacket> packetQueue; /*!< Queue to keep the incoming frames */
        int maxQueueLength; /*!< Maximum number of frames that can be present inside the queue */

    public:

        /**
            @brief Constructor of class PacketManager.
            @param streamID ID of the associated camera stream.
            @param maxQueueLength maximum number of packets that can exist in the queue.
        */
        PacketManager(int streamID, int maxQueueLength);

        /**
            @brief To add a packet at the end of queue.
            @param newFrame The packet that should be added.
        */
        void enqueuePacket(AVPacket newPacket);


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
        void dropPacket();

};

#endif