#include "processor.h"


void Processor::process() {

    XInitThreads();
    streamReceiver1 = new std::thread(StreamReceiver(),std::ref(videoStreams));
    decoder1 = new std::thread(Decoder(),std::ref(videoStreams));
	display1 = new std::thread(display(), std::ref(videoStreams));
    outputStreamer1 = new std::thread(Streamer(), std::ref(videoStreams));

}

void Processor::addStream(VideoStream* videoStream) {

    videoStream->OpenStream();
    videoStreams.emplace_back(videoStream);

}
 
void Processor::join() {

    streamReceiver1->join();

    for (int i; i < videoStreams.size(); i++) {

        videoStreams[i]->CloseStream();

    }

}