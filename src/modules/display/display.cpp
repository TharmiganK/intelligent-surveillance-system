/**
   Intelligent Surveillance System
   @file display.cpp
   @author Lavinan Selvaratnam
*/

#include "display.h"

/**
   @details This is a temprary function which runs as a seperate thread where it takes the 
   frames (if there is any) from the frame queue passed as argument and displays it. After displaying
   a frame, it waits for 200 milliseconds to introduce a delay and to see how queueing works.
*/
void display::operator()(FrameManager *frameManager) {

    cv::Mat frameToDisplay;

    while(true) {

        if (!frameManager->queueIsEmpty()) {

            frameToDisplay = frameManager->dequeueFrame();
            cv::imshow("frame", frameToDisplay);
            cv::waitKey(200);

        }

    }

}