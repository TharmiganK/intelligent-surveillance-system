/**
    Intelligent Surveillance System
    @file processor.cpp
    @author Lavinan Selvaratnam
*/

#include "processor.h"

/**
    @details process function initiate the threads and start process to receive, 
    decode and display the frames from camera streams listed in videoStreams vector
*/
void Processor::process() {

    XInitThreads();
    streamReceiver1 = new std::thread(StreamReceiver(),std::ref(videoStreams));
    decoder1 = new std::thread(Decoder(),std::ref(videoStreams));
	// display1 = new std::thread(display(), std::ref(videoStreams));
    outputStreamer1 = new std::thread(Streamer(), std::ref(videoStreams));

}

/**
    @details addStream function accepts pointer of VideoStream object which it 
    will open and add it to the videoStream vector in order to process it using 
    this processor.
*/
void Processor::addStream(VideoStream* videoStream) {

    videoStream->OpenStream();
    videoStreams.emplace_back(videoStream);

}

/**
    @details join function waits until all the threads finish processing and 
    close the VideoStreams which are in videoStream vector.
*/
void Processor::join() {

    streamReceiver1->join();

    for (int i; i < videoStreams.size(); i++) {

        videoStreams[i]->CloseStream();

    }

}