/**
    Intelligent Surveillance System
    @file display.h
    @author Lavinan Selvaratnam
*/

#include "../videoStream/videoStream.h"
#include <boost/log/trivial.hpp>
#include <chrono>

/**
    @class Class to take frames from frame queue and display them.
    @details This is a temporary class to show the intermediate output after decoding and
    storing the frames in a frame queue.
*/
class display {

    public:

        /**
            @brief Member function to run as a seperate thread and display the frames.
            @param frameQueue reference to the frame queue from which frames should be taken.
        */
        void operator()(VideoStream videoStreams[], int numberOfStreams);
        
};