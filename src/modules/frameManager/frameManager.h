#ifndef _FRAMEMANAGER_H
#define _FRAMEMANAGER_H
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <deque>
#include <mutex>

class FrameManager {

    public:
        int streamID;
        std::deque<cv::Mat>frameQueue;
        int maxQueueLength;

        FrameManager(int streamID, int maxQueueLength);

        void enqueueFrame(cv::Mat newFrame);

        cv::Mat dequeueFrame();

        int queueLength();

        bool queueIsEmpty();

        void dropFrame();

};

#endif