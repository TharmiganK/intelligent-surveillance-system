/**
    Intelligent Surveillance System
    @file frameManager.cpp
    @author Lavinan Selvaratnam
*/

/**
    @todo Abstract as a generic queue manager class
    @todo unit test
*/

#include "frameManager.h"

/**
    @details Constructor of FrameManager to define the camera stream ID associated and to
    define the maximum number of frames that should be allowed to be present in the queue.
    Memory for the queue is also allocated with a size of queueCapacity.
*/
FrameManager::FrameManager(int streamID, int queueCapacity) {

    this->streamID = streamID;
    this->queueCapacity = queueCapacity;
    frameQueue = boost::circular_buffer<cv::Mat>(queueCapacity);

}

/**
    @details Adds the frame passed as argument at the end of the frame queue.
    If the queue is full it drops a frame and then add the new one.
    @sa dropFrame()
*/
void FrameManager::enqueueFrame(cv::Mat newFrame) {

    if(FrameManager::queueLength() < FrameManager::queueCapacity) {

        std::lock_guard<std::mutex> guard(mutexForQueue);
        frameQueue.push_back(newFrame);

    } else {

        FrameManager::dropFrame();
        std::lock_guard<std::mutex> guard(mutexForQueue);
        frameQueue.push_back(newFrame);

    }
            
}

/**
    @details Takes the first frame out of the queue and returns it
*/
cv::Mat FrameManager::dequeueFrame() {

    std::lock_guard<std::mutex> guard(mutexForQueue);
    cv::Mat frame = frameQueue.front();
    frameQueue.pop_front();
    return frame;

}

/**
    @details Checks the number of frames currently present inside the queue
*/
int FrameManager::queueLength() {

    std::lock_guard<std::mutex> guard(mutexForQueue);
    return frameQueue.size();

}

/**
    @details Checks whether there is any frame present inside the queue
*/
bool FrameManager::queueIsEmpty() {

    std::lock_guard<std::mutex> guard(mutexForQueue);
    return frameQueue.empty();

}

/**
    @details Drops the first frame inside the queue. This is used to make space
    before adding a new one when the queue is full
    @sa enqueueFrame()
    @todo change the logic to drop a frame based on difference
*/
void FrameManager::dropFrame() {

    std::lock_guard<std::mutex> guard(mutexForQueue);
    frameQueue.pop_front();
    BOOST_LOG_TRIVIAL(info) << "Frame queue is full. One frame dropped";

}
