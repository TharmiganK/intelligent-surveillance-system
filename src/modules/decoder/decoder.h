#include "../frameManager/frameManager.h"
#include "../packetManager/packetManager.h"
#include <fstream>
#include <deque>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavformat/avio.h>
#include <libswscale/swscale.h>
}
class Decoder {

    public:
        AVCodecContext *codec_ctx;
        Decoder(AVCodecContext *codec_ctx);
        void operator()(FrameManager *frameManager, PacketManager *packetManager);

};