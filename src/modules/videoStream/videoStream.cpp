/**
    Intelligent Surveillance System
    @file videoStream.cpp
    @author Tharmigan Krishnananthalingam , Lavinan Selvaratnam & Sanoojan Baliah
*/

#include "videoStream.h"

/**
    @details Constructor of VideoStream to define the attributes of a camera stream with
    default values and to initialize queues for the stream with queueCapacity.
*/
VideoStream::VideoStream(int streamID, const char *streamURL, int queueCapacity) :

	streamID(streamID),
	streamURL(streamURL),
	frameQueue(streamID, queueCapacity),
	fgMaskQueue(streamID, queueCapacity),
	opticalFlowQueue(streamID, queueCapacity),
	packetQueue(streamID, queueCapacity),
	videoBaseTime(0.0),
	videoFPS(0.0),
	isOpen(false),
	videoStreamIndex(-1),
	videoCodecCtx(NULL),
	formatCtx(NULL),
	outputURL("rtmp://localhost/live/"+std::to_string(streamID)),
	bitrate(1000000),
	codec_profile("high444")
    {
	av_log_set_level(AV_LOG_DEBUG);
	
    }

/**
    @details OpenStream method opens the RTSP stream by accepting the RTSP
    URL and configuresfps and basetime in addition to format context of 
    the stream.
*/
bool VideoStream::OpenStream(){

    CloseStream();

    // Register all components
    av_register_all();
    avformat_network_init();

    // Open media stream.
    if (avformat_open_input(&formatCtx, streamURL, NULL, NULL) != 0){

        CloseStream();
        return false;

    }

    // Get format info.
    if (avformat_find_stream_info(formatCtx, NULL) < 0){

        CloseStream();
        return false;

    }

    // open video stream.
    bool hasVideo = OpenVideoStream();

    if (!hasVideo){

        CloseStream();
        return false;

    }

    isOpen = true;

    // Get stream information.
    if (videoStreamIndex != -1){

        videoFPS            = av_q2d(formatCtx->streams[videoStreamIndex]->r_frame_rate);
        // Need for convert time to ffmpeg time.
        videoBaseTime       = av_q2d(formatCtx->streams[videoStreamIndex]->time_base); 

    }


    width = videoCodecCtx->width;
    height = videoCodecCtx->height;

    bufferImgSize = avpicture_get_size(AV_PIX_FMT_BGR24, videoCodecCtx->width, videoCodecCtx->height);
    buffer = (uint8_t*) av_mallocz(bufferImgSize);

    imgCvtCtx = sws_getContext(videoCodecCtx->width, videoCodecCtx->height, videoCodecCtx->pix_fmt, 
                                videoCodecCtx->width, videoCodecCtx->height, AV_PIX_FMT_BGR24,
                                SWS_BICUBIC, NULL, NULL, NULL);

    videoCodec = avcodec_find_decoder(videoCodecCtx->codec_id);

    if (videoCodec)
    {
        avcodec_open2(videoCodecCtx, videoCodec, NULL);
    }
	initializeOutputStream();
    return true;

}

/**
    @details OpenStream method closes the RTSP stream and close the 
    format context for the stream if created.
*/
bool VideoStream::CloseStream(){

    isOpen = false;

    // Close video.
    CloseVideoStream();

    if (formatCtx){

        avformat_close_input(&formatCtx);
        formatCtx = NULL;

    }
	

    return true;

}

/**
    @details OpenStream method searches for video stream and configures
    the video stream index and obtains the video codec information from 
    the video stream and update the videoCodecCtx.
*/
bool VideoStream::OpenVideoStream(){

    bool res = false;

    if (formatCtx){

        videoStreamIndex = -1;

        for (unsigned int i = 0; i < formatCtx->nb_streams; i++){

            if (formatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO){

                videoStreamIndex = i;
                videoCodecCtx = formatCtx->streams[i]->codec;
                res = true;

                break;

            }
        }
    }

    return res;

}

/**
    @details CloseVideoStream closes the video codec context if created
    and removes the videoStreamIndex;
*/
void VideoStream::CloseVideoStream(){

    if (videoCodecCtx){

        avcodec_close(videoCodecCtx);
        videoCodecCtx = NULL;
        videoStreamIndex = -1;

    }

}

void  VideoStream::initialize_avformat_context(AVFormatContext *&fctx, const char *format_name)
{
	int ret = avformat_alloc_output_context2(&fctx, nullptr, format_name, nullptr);
	if (ret < 0)
	{
		std::cout << "Could not allocate output format context!" << std::endl;
		exit(1);
	}
}

void VideoStream::initialize_io_context(AVFormatContext *&fctx, const char *output)
{
	if (!(fctx->oformat->flags & AVFMT_NOFILE))
	{
		int ret = avio_open2(&fctx->pb, output, AVIO_FLAG_WRITE, nullptr, nullptr);
		if (ret < 0)
		{
			std::cout << "Could not open output IO context!" << std::endl;
			exit(1);
		}
	}
}

void VideoStream::set_codec_params(AVFormatContext *&fctx, AVCodecContext *&codec_ctx, double width, double height, int fps, int bitrate)
{
	const AVRational dst_fps = { fps, 1 };

	codec_ctx->codec_tag = 0;
	codec_ctx->codec_id = AV_CODEC_ID_H264;
	codec_ctx->codec_type = AVMEDIA_TYPE_VIDEO;
	codec_ctx->width = width;
	codec_ctx->height = height;
	codec_ctx->gop_size = 12;
	codec_ctx->pix_fmt = AV_PIX_FMT_YUV444P;
	codec_ctx->framerate = dst_fps;
	codec_ctx->time_base = av_inv_q(dst_fps);
	codec_ctx->bit_rate = bitrate;
	if (fctx->oformat->flags & AVFMT_GLOBALHEADER)
	{
		codec_ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
	}
}

void VideoStream::initialize_codec_stream(AVStream *&stream, AVCodecContext *&codec_ctx, AVCodec *&codec, std::string codec_profile)
{
	int ret = avcodec_parameters_from_context(stream->codecpar, codec_ctx);
	if (ret < 0)
	{
		std::cout << "Could not initialize stream codec parameters!" << std::endl;
		exit(1);
	}

	AVDictionary *codec_options = nullptr;
	av_dict_set(&codec_options, "profile", codec_profile.c_str(), 0);
	av_dict_set(&codec_options, "preset", "superfast", 0);
	av_dict_set(&codec_options, "tune", "zerolatency", 0);

	// open video encoder
	ret = avcodec_open2(codec_ctx, codec, &codec_options);
	if (ret < 0)
	{
		std::cout << "Could not open video encoder!" << std::endl;
		exit(1);
	}
}

SwsContext *VideoStream::initialize_sample_scaler(AVCodecContext *codec_ctx, double width, double height)
{
	SwsContext *swsctx = sws_getContext(width, height, AV_PIX_FMT_BGR24, width, height, codec_ctx->pix_fmt, SWS_BICUBIC, nullptr, nullptr, nullptr);
	if (!swsctx)
	{
		std::cout << "Could not initialize sample scaler!" << std::endl;
		exit(1);
	}

	return swsctx;
}

AVFrame *VideoStream::allocate_frame_buffer(AVCodecContext *codec_ctx, double width, double height)
{
	AVFrame *frame = av_frame_alloc();
	int ret = av_image_alloc(frame->data, frame->linesize, width, height, AV_PIX_FMT_BGR24, 1);
	if (ret < 0) {
		std::cout << "Could not allocate image!" << std::endl;
	}


	std::vector<uint8_t> framebuf(av_image_get_buffer_size(codec_ctx->pix_fmt, width, height, 1));
	av_image_fill_arrays(frame->data, frame->linesize, framebuf.data(), codec_ctx->pix_fmt, width, height, 1);
	frame->width = width;
	frame->height = height;
	frame->format = static_cast<int>(codec_ctx->pix_fmt);

	return frame;
}

void VideoStream::initializeOutputStream()
{
#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(58, 9, 100)
	av_register_all();
#endif
	avformat_network_init();

	const char *output = outputURL.c_str();
	int ret;
	ofmt_ctx = nullptr;
	out_codec = nullptr;
	out_stream = nullptr;
	out_codec_ctx = nullptr;

	initialize_avformat_context(ofmt_ctx, "flv");
	initialize_io_context(ofmt_ctx, output);

	out_codec = avcodec_find_encoder(AV_CODEC_ID_H264);
	out_stream = avformat_new_stream(ofmt_ctx, out_codec);
	out_codec_ctx = avcodec_alloc_context3(out_codec);

	set_codec_params(ofmt_ctx, out_codec_ctx, width, height, 25, bitrate);
	initialize_codec_stream(out_stream, out_codec_ctx, out_codec, codec_profile);

	out_stream->codecpar->extradata = out_codec_ctx->extradata;
	out_stream->codecpar->extradata_size = out_codec_ctx->extradata_size;

	av_dump_format(ofmt_ctx, 0, output, 1);
	//swsctx = initialize_sample_scaler(out_codec_ctx, width, height);
	frame = allocate_frame_buffer(out_codec_ctx, width, height);


	ret = avformat_write_header(ofmt_ctx, nullptr);
	if (ret < 0)
	{
		std::cout << "Could not write header!" << std::endl;
		exit(1);
	}
}