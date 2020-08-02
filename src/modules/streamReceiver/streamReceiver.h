//This is a mock stream receiver

#include "../frameManager/frameManager.h"
#include "../packetManager/packetManager.h"
#include "../decoder/decoder.h"
#include <fstream>
#include <thread>
#include <X11/Xlib.h>
#include <deque>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavformat/avio.h>
#include <libswscale/swscale.h>
}

class StreamReceiver {
    public:
        AVCodecContext *codec_ctx;
        void setContext(AVCodecContext *codec_ctx);
        void operator()(PacketManager *packetManager, FrameManager *frameManager);
};
