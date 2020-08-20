/**
    Intelligent Surveillance System
    @file display.h
    @author Lavinan Selvaratnam
*/

#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "../videoStream/videoStream.h"
#include <boost/log/trivial.hpp>
#include <chrono>
#include "tbb/concurrent_vector.h"

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
        void operator()(tbb::concurrent_vector< std::shared_ptr< VideoStream >>& videoStreams);
        
};

#endif