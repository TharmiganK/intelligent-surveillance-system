/**
    Intelligent Surveillance System
    @file decoder.h
    @author Tharmigan Krishnananthalingam
*/

#ifndef _DECODER_H
#define _DECODER_H
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

using namespace std;

/**
    @class Class to decode video packets
    @details Decoder contains a video codec to decode AVPackets to AVFrames.
    Swsscale function is used to convert YUV to BGR. And OpenCV Mat function
    is used to convert BGR frame to image Matrix.
*/
class Decoder {

    private:

        AVCodecContext *codec_ctx; /* Codec context information of the video codec */
        struct SwsContext *imgCvtCtx; /* Image convert context information (YUV -> BGR) */
        int width; /* Width of the decoded frame */
        int height; /* Height of the decoded frame */
        int bufferImgSize; /* Image buffer size from video codec */
        uint8_t *buffer; /* Buffer image used for YUV to BGR conversion */
        AVCodec* videoCodec; /* Video codec decoder */

    public:

        /**
            @brief Constructor of class Decoder.
            @param codec_context Video codec context.
            @details Constructor of Decoder defines the video codec context used for decoding, height and width for
            the decoded frame, buffer image used to convert YUV to BGR and image convert context for YUV to BGR
            conversion.
        */
        Decoder(AVCodecContext *codec_context){

            codec_ctx = codec_context;

            width = codec_ctx->width;
            height = codec_ctx->height;

            bufferImgSize = avpicture_get_size(AV_PIX_FMT_BGR24, codec_ctx->width, codec_ctx->height);
            buffer = (uint8_t*) av_mallocz(bufferImgSize);

            imgCvtCtx = sws_getContext(codec_ctx->width, Decoder::codec_ctx->height,Decoder::codec_ctx->pix_fmt, 
                                        Decoder::codec_ctx->width, Decoder::codec_ctx->height, AV_PIX_FMT_BGR24,
                                        SWS_BICUBIC, NULL, NULL, NULL);

            videoCodec = avcodec_find_decoder(codec_ctx->codec_id);

            if (videoCodec)
            {
                avcodec_open2(codec_ctx, videoCodec, NULL);
            }

        }

        /**
            @brief Method to Decode video packet into YUV frame.
            @return Decoded YUV frame.
            @param avpkt Video Packet from the stream.
        */
        AVFrame *DecodeVideo(const AVPacket *avpkt);

        /**
            @brief Method to convert YUV frame to BGR frame.
            @return Decoded BGR frame.
            @param frameYUV Decoded YUV frame.
        */
        AVFrame *GetBGRFrame(AVFrame *frameYUV);

        /**
            @brief Method to convert BGR frame to Opencv Mat image.
            @return OpenCV Mat image.
            @param frameBGR Decoded BGR frame.
        */
        cv::Mat GetImage(AVFrame *frameBGR);

    public:

        void operator()(FrameManager *frameManager, PacketManager *packetManager);

};

#endif