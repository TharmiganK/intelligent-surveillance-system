#include "frameManager.h"

FrameManager::FrameManager(int streamID, int maxQueueLength) {

    this->FrameManager::streamID = streamID;
    this->FrameManager::maxQueueLength = maxQueueLength;

}

void FrameManager::enqueueFrame(cv::Mat newFrame) {

    if(FrameManager::queueLength() < FrameManager::maxQueueLength) {
        // std::cout << "FREE SPACE" << std::endl;
        frameQueue.push_back(newFrame);
    }
    else {
        FrameManager::dropFrame();
        frameQueue.push_back(newFrame);
    }
            
}

cv::Mat FrameManager::dequeueFrame() {

    // std::lock_guard<std::mutex> guard(myMutex);
    cv::Mat frame = frameQueue.front();
    frameQueue.pop_front();
    return frame;

}

int FrameManager::queueLength() {

    // std::lock_guard<std::mutex> guard(myMutex);
    return frameQueue.size();

}

bool FrameManager::queueIsEmpty() {

    // std::lock_guard<std::mutex> guard(myMutex);
    return frameQueue.empty();

}

void FrameManager::dropFrame() {

    frameQueue.pop_front();
    std::cout << "FRAME DROPPED" << std::endl;

}
