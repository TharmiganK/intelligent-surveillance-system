#include "../frameManager/frameManager.h"
#include <fstream>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavformat/avio.h>
#include <libswscale/swscale.h>
}
class decoder {

    public:
        void operator()(FrameManager *m1);

};