/**
    Intelligent Surveillance System
    @file initialVideoAnalyticsUnit.cpp
    @author Tharmigan Krishnanathalingam
*/

#include "initialVideoAnalyticsUnit.h"
#define MAX_NUM_FRAMES 5000

void InitialVideoAnalyticsUnit::operator()(VideoStream& videoStream) {
    int count = 0;
    while (count < MAX_NUM_FRAMES) {

        auto frame = videoStream.frameQueue.dequeueFrame();

        std::thread backgroundExtractor1( BackgroundExtractor(False, 0), std::ref(videoStream), std::ref(frame));
        std::thread opticalFlowEstimator1( OpticalFlowEstimator(False), std::ref(videoStream), std::ref(frame));

        count ++;
    }
}