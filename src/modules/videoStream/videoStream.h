#include "../frameManager/frameManager.h"
#include "../packetManager/packetManager.h"
#include <string>

struct VideoStream {

    int streamID;
    const char *streamURL;
    FrameManager* frameManager;
    PacketManager* packetManager;

    VideoStream(int streamID, const char *streamURL);

};