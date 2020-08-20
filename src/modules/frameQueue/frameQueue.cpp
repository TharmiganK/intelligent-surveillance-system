/**
    Intelligent Surveillance System
    @file frameQueue.cpp
    @author Lavinan Selvaratnam
*/

/**
    @todo Abstract PacketQueue and FrameQueue as a generic queue class
    @todo unit test
*/

#include "frameQueue.h"

/**
    @details Constructor of FrameQueue to define the camera stream ID associated and to
    define the number of frames that should be allowed to be present in the queue.
    Memory for the queue is also allocated with a size of queueCapacity.
*/
FrameQueue::FrameQueue(int queueCapacity) :

    queueCapacity(queueCapacity),
    queue(queueCapacity)

    {

    }

/**
    @details Adds the frame passed as argument at the end of the frame queue.
    If the queue is full it drops a frame and then add the new one.
    @sa dropFrame()
*/
bool FrameQueue::enqueueFrame(cv::Mat newFrame) {

    if(FrameQueue::queueLength() < FrameQueue::queueCapacity) {

        std::lock_guard<std::mutex> guard(mutexForQueue);
        queue.push_back(newFrame);

    } else {

        if (FrameQueue::dropFrame()) {

            std::lock_guard<std::mutex> guard(mutexForQueue);
            queue.push_back(newFrame);

        } else {

            BOOST_LOG_TRIVIAL(error) << "Frame queue doesn't have space";
            return false;

        }

    }

    return true;
            
}

/**
    @details Takes the first frame out of the queue and returns it
*/
cv::Mat FrameQueue::dequeueFrame() {

    std::lock_guard<std::mutex> guard(mutexForQueue);
    currentFrame = queue.front();
    queue.pop_front();
    return currentFrame;

}

/**
    @details Checks the number of frames currently present inside the queue
*/
int FrameQueue::queueLength() {

    std::lock_guard<std::mutex> guard(mutexForQueue);
    return queue.size();

}

/**
    @details Checks whether there is any frame present inside the queue
*/
bool FrameQueue::queueIsEmpty() {

    std::lock_guard<std::mutex> guard(mutexForQueue);
    return queue.empty();

}

/**
    @details Drops the first frame inside the queue. This is used to make space
    before adding a new one when the queue is full
    @sa enqueueFrame()
    @todo change the logic to drop a frame based on difference
*/
bool FrameQueue::dropFrame() {

    if(!queueIsEmpty()) {

        std::lock_guard<std::mutex> guard(mutexForQueue);
        queue.pop_front();
        BOOST_LOG_TRIVIAL(info) << "A frame is dropped";

        return true;

    }

    return false;

}
