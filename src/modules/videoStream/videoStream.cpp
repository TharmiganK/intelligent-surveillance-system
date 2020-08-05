#include "videoStream.h"

VideoStream::VideoStream(int streamID, const char *streamURL) {

    this->streamID = streamID;
    this->streamURL = streamURL;
    frameManager = new FrameManager(1,30);
    packetManager = new PacketManager(1,30);

}