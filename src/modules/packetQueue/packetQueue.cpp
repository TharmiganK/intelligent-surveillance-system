/**
    Intelligent Surveillance System
    @file packetQueue.cpp
    @author Lavinan Selvaratnam
*/

/**
    @todo Abstract PacketQueue and FrameQueue as a generic queue class
    @todo unit test
*/

#include "packetQueue.h"

/**
    @details Constructor of PacketQueue to define the camera stream ID associated and to
    define the number of packets that should be allowed to be present in the queue.
    Memory for the queue is also allocated with a size of queueCapacity.
*/
PacketQueue::PacketQueue(int streamID, int queueCapacity) :

    streamID(streamID),
    queueCapacity(queueCapacity),
    queue(queueCapacity)
    
    {

    }

/**
    @details Adds the packet passed as argument at the end of the frame queue.
    If the queue is full it drops a packet and then add the new one.
    @sa dropPacket()
*/
void PacketQueue::enqueuePacket(AVPacket newPacket) {

    if(PacketQueue::queueLength() < PacketQueue::queueCapacity) {

        std::lock_guard<std::mutex> guard(mutexForQueue);
        queue.push_back(newPacket);

    } else {

        PacketQueue::dropPacket();
        std::lock_guard<std::mutex> guard(mutexForQueue);
        queue.push_back(newPacket);
        
    }
            
}

/**
    @details Takes the first packet out of the queue and returns it
*/
AVPacket PacketQueue::dequeuePacket() {

    std::lock_guard<std::mutex> guard(mutexForQueue);
    AVPacket packet = queue.front();
    queue.pop_front();
    return packet;

}

/**
    @details Checks the number of packet currently present inside the queue
*/
int PacketQueue::queueLength() {

    std::lock_guard<std::mutex> guard(mutexForQueue);
    return queue.size();

}

/**
    @details Checks whether there is any packet present inside the queue
*/
bool PacketQueue::queueIsEmpty() {

    std::lock_guard<std::mutex> guard(mutexForQueue);
    return queue.empty();

}

/**
    @details Drops the first packet inside the queue. This is used to make space
    before adding a new one when the queue is full
    @sa enqueuePacket()
    @todo change the logic to drop a frame based on difference
*/
void PacketQueue::dropPacket() {

    std::lock_guard<std::mutex> guard(mutexForQueue);
    queue.pop_front();
    BOOST_LOG_TRIVIAL(info) << "Packet queue is full. One packet dropped";

}
