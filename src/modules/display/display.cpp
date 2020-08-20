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
void display::operator()(tbb::concurrent_vector< std::shared_ptr< VideoStream >>& videoStreams) {

    cv::Mat frameToDisplay;
    int count = 0;
    //std::chrono::high_resolution_clock::time_point t1;
    //std::chrono::high_resolution_clock::time_point t2;
    double totalTime = 0;

    while(count < MAX_NUM_FRAMES) {

        //t1 = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < videoStreams.size(); i++) {

            if (!videoStreams[i]->frameQueue.queueIsEmpty()) {

                frameToDisplay = videoStreams[i]->frameQueue.dequeueFrame();
                //cv::imshow("frame", frameToDisplay);
                cv::waitKey(20);
                count++;
                //t2 = std::chrono::high_resolution_clock::now();
                //totalTime = totalTime + std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count();
                //BOOST_LOG_TRIVIAL(info) << "FPS : " << count/(totalTime * numberOfStreams);
                //t1 = std::chrono::high_resolution_clock::now();

            }

        }

    }

    BOOST_LOG_TRIVIAL(info) << "DISPLAYING IS FINISHED";

}