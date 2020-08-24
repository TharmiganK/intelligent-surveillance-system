/**
    Intelligent Surveillance System
    @file processor.h
    @author Lavinan Selvaratnam
*/

#include "../streamReceiver/streamReceiver.h"
#include "../decoder/decoder.h"
#include "../display/display.h"
#include "../videoStream/videoStream.h"
#include <vector>
#include "tbb/concurrent_vector.h"
#include <thread>
#include <X11/Xlib.h>
#include <boost/log/trivial.hpp>

/**
    @class Class to create threads, add camera streams and start process.
    @details Processor contains three threads to receive decode and display the 
    frames. It has a vector of camera streams which will be processed by the threads.
    Camera streams can be added dynamically to the vector in order to process them.
*/
class Processor {

    tbb::concurrent_vector< std::shared_ptr< VideoStream >> videoStreams; /*!< A vector which has the pointers of camera streams */
    std::thread* streamReceiver1; /*!< A thread to receive the frames of VideoStreams */
    std::thread* decoder1; /*!< A thread to decode compressed frames received by StreamReciever */
	std::thread* display1; /*!< A thread to display frames decoded by Decoder */

    public:
    
        /**
            @brief Function to start all the threads.
        */
        void process();

        /**
            @brief Add a new stream to the VideoStream vector.
            @param VideoStream* pointer of the video stream created.
        */
        void addStream(VideoStream* videoStream);

        /**
            @brief Wait until the process by threads are over.
        */
        void join();

};