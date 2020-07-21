#include "frameManager.h"

void manager::enqueueFrame(cv::Mat newFrame) {

    if(manager::queueLength() < 30) {
        // std::cout << "FREE SPACE" << std::endl;
        frameQueue.push_back(newFrame);
    }
    else {
        manager::dropFrame();
        frameQueue.push_back(newFrame);
    }
            
}

cv::Mat manager::dequeueFrame() {

    // std::lock_guard<std::mutex> guard(myMutex);
    cv::Mat frame = frameQueue.front();
    frameQueue.pop_front();

}

int manager::queueLength() {

    // std::lock_guard<std::mutex> guard(myMutex);
    return frameQueue.size();

}

bool manager::queueIsEmpty() {

    // std::lock_guard<std::mutex> guard(myMutex);
    return frameQueue.empty();

}

void manager::dropFrame() {

    frameQueue.pop_front();
    std::cout << "FRAME DROPPED" << std::endl;

}
