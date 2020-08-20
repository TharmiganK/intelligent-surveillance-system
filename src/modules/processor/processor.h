#include "../streamReceiver/streamReceiver.h"
#include "../decoder/decoder.h"
#include "../display/display.h"
#include "../videoStream/videoStream.h"
#include <vector>
#include "tbb/concurrent_vector.h"
#include <thread>
#include <X11/Xlib.h>
#include <boost/log/trivial.hpp>

class Processor {

    tbb::concurrent_vector< std::shared_ptr< VideoStream >> videoStreams;
    std::thread* streamReceiver1;
    std::thread* decoder1;
	std::thread* display1;

    public:
    
        void process();
        void addStream(VideoStream* videoStream);
        void join();

};