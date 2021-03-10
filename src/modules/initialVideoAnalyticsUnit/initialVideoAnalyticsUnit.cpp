/**
    Intelligent Surveillance System
    @file initialVideoAnalyticsUnit.cpp
    @author Tharmigan Krishnanathalingam
*/

#include "initialVideoAnalyticsUnit.h"
#define MAX_NUM_FRAMES 5000

void InitialVideoAnalyticsUnit::operator()(VideoStream& videoStream) {
    int count = 0;
    OpticalFlowEstimator opticalFlowEstimator(true);
    BackgroundExtractor backgroundExtractor(true, 0);

    while (count < MAX_NUM_FRAMES) {

        if (!videoStream.frameQueue.queueIsEmpty()) {

            auto frame = videoStream.frameQueue.dequeueFrame();

            // std::thread backgroundExtractor1( BackgroundExtractor(false, 0), std::ref(videoStream), std::ref(frame));
            opticalFlowEstimator.compute(std::ref(videoStream), frame);
            backgroundExtractor.compute(std::ref(videoStream), frame);
            // opticalFlowEstimator1.join();

            count ++;
            
        }
    }
}