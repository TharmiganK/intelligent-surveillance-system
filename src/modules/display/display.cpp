/**
    Intelligent Surveillance System
    @file display.cpp
    @author Lavinan Selvaratnam
*/

#include "display.h"

#define MAX_NUM_FRAMES 1000

/**
    @details This is a temprary function which runs as a seperate thread where it takes the 
    frames (if there is any) from the frame queue passed as argument and displays it. After displaying
    a frame, it waits for 200 milliseconds to introduce a delay and see how queueing works.
*/
void display::operator()(FrameQueue& frameQueue) {

    cv::Mat frameToDisplay;
    int count = 0;

    while(count < MAX_NUM_FRAMES) {

        if (!frameQueue.queueIsEmpty()) {

            frameToDisplay = frameQueue.dequeueFrame();
            cv::imshow("frame", frameToDisplay);
            cv::waitKey(20);

            count++;

        }

    }

}