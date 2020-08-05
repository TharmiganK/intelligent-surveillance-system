/**
    Intelligent Surveillance System
    @file packetManager.cpp
    @author Lavinan Selvaratnam
*/

/**
    @todo Abstract as a generic queue manager class
    @todo unit test
*/

#include "packetManager.h"

/**
    @details Constructor of PacketManager to define the camera stream ID associated and to
    define the maximum number of packets that should be allowed to be present in the queue.
    Memory for the queue is also allocated with a size of queueCapacity.
*/
PacketManager::PacketManager(int streamID, int queueCapacity) {

    this->streamID = streamID;
    this->queueCapacity = queueCapacity;
    packetQueue  = boost::circular_buffer<AVPacket>(queueCapacity);

}

/**
    @details Adds the packet passed as argument at the end of the frame queue.
    If the queue is full it drops a packet and then add the new one.
    @sa dropPacket()
*/
void PacketManager::enqueuePacket(AVPacket newPacket) {

    if(PacketManager::queueLength() < PacketManager::queueCapacity) {

        std::lock_guard<std::mutex> guard(mutexForQueue);
        packetQueue.push_back(newPacket);

    } else {

        PacketManager::dropPacket();
        std::lock_guard<std::mutex> guard(mutexForQueue);
        packetQueue.push_back(newPacket);
        
    }
            
}

/**
    @details Takes the first packet out of the queue and returns it
*/
AVPacket PacketManager::dequeuePacket() {

    std::lock_guard<std::mutex> guard(mutexForQueue);
    AVPacket packet = packetQueue.front();
    packetQueue.pop_front();
    return packet;

}

/**
    @details Checks the number of packet currently present inside the queue
*/
int PacketManager::queueLength() {

    std::lock_guard<std::mutex> guard(mutexForQueue);
    return packetQueue.size();

}

/**
    @details Checks whether there is any packet present inside the queue
*/
bool PacketManager::queueIsEmpty() {

    std::lock_guard<std::mutex> guard(mutexForQueue);
    return packetQueue.empty();

}

/**
    @details Drops the first packet inside the queue. This is used to make space
    before adding a new one when the queue is full
    @sa enqueuePacket()
    @todo change the logic to drop a frame based on difference
*/
void PacketManager::dropPacket() {

    std::lock_guard<std::mutex> guard(mutexForQueue);
    packetQueue.pop_front();
    BOOST_LOG_TRIVIAL(info) << "Packet queue is full. One packet dropped";

}
