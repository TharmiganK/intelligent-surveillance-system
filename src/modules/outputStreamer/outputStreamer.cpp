/**
	Intelligent Surveillance System
	@file outputStreamer.cpp
	@author Sanoojan Baliah
*/


#include "outputStreamer.h"
#define MAX_NUM_FRAMES 1000


void initialize_avformat_context(AVFormatContext *&fctx, const char *format_name)
{
	int ret = avformat_alloc_output_context2(&fctx, nullptr, format_name, nullptr);
	if (ret < 0)
	{
		std::cout << "Could not allocate output format context!" << std::endl;
		exit(1);
	}
}

void initialize_io_context(AVFormatContext *&fctx, const char *output)
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
uchar* bgr_to_yuv420p(unsigned  char* yuv420p, unsigned char* bgr, int width, int height)
{
	if (yuv420p == NULL || bgr == NULL)
		return 0;
	int frameSize = width * height;
	int chromaSize = frameSize / 4;

	int yIndex = 0;
	int uIndex = frameSize;
	int vIndex = frameSize + chromaSize;

	int R, G, B, Y, U, V;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			B = bgr[(i * width + j) * 3 + 0];
			G = bgr[(i * width + j) * 3 + 1];
			R = bgr[(i * width + j) * 3 + 2];

			//BGR to YUV
			Y = ((66 * R + 129 * G + 25 * B + 128) >> 8) + 16;
			U = ((-38 * R - 74 * G + 112 * B + 128) >> 8) + 128;
			V = ((112 * R - 94 * G - 18 * B + 128) >> 8) + 128;

			yuv420p[yIndex++] = (unsigned char)((Y < 0) ? 0 : ((Y > 255) ? 255 : Y));
			if (i % 2 == 0 && j % 2 == 0)
			{
				yuv420p[uIndex++] = (unsigned char)((U < 0) ? 0 : ((U > 255) ? 255 : U));
				yuv420p[vIndex++] = (unsigned char)((V < 0) ? 0 : ((V > 255) ? 255 : V));
			}
		}
	}
	return yuv420p;
}


void set_codec_params(AVFormatContext *&fctx, AVCodecContext *&codec_ctx, double width, double height, int fps, int bitrate)
{
	const AVRational dst_fps = { fps, 1 };

	codec_ctx->codec_tag = 0;
	codec_ctx->codec_id = AV_CODEC_ID_H264;
	codec_ctx->codec_type = AVMEDIA_TYPE_VIDEO;
	codec_ctx->width = width;
	codec_ctx->height = height;
	codec_ctx->gop_size = 12;
	codec_ctx->pix_fmt = AV_PIX_FMT_YUV420P;
	codec_ctx->framerate = dst_fps;
	codec_ctx->time_base = av_inv_q(dst_fps);
	codec_ctx->bit_rate = bitrate;
	if (fctx->oformat->flags & AVFMT_GLOBALHEADER)
	{
		codec_ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
	}
}

void initialize_codec_stream(AVStream *&stream, AVCodecContext *&codec_ctx, AVCodec *&codec, std::string codec_profile)
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

SwsContext *initialize_sample_scaler(AVCodecContext *codec_ctx, double width, double height)
{
	SwsContext *swsctx = sws_getContext(width, height, AV_PIX_FMT_BGR24, width, height, codec_ctx->pix_fmt, SWS_BICUBIC, nullptr, nullptr, nullptr);
	if (!swsctx)
	{
		std::cout << "Could not initialize sample scaler!" << std::endl;
		exit(1);
	}

	return swsctx;
}

AVFrame *allocate_frame_buffer(AVCodecContext *codec_ctx, double width, double height)
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

void write_frame(AVCodecContext *codec_ctx, AVFormatContext *fmt_ctx, AVFrame *frame)
{
	AVPacket pkt = { 0 };
	av_init_packet(&pkt);

	int ret = avcodec_send_frame(codec_ctx, frame);
	if (ret < 0)
	{
		std::cout << "Error sending frame to codec context!" << std::endl;
		exit(1);
	}

	ret = avcodec_receive_packet(codec_ctx, &pkt);
	if (ret < 0)
	{
		std::cout << "Error receiving packet from codec context!" << std::endl;
		exit(1);
	}

	av_interleaved_write_frame(fmt_ctx, &pkt);
	av_packet_unref(&pkt);
}




void Streamer::operator()(FrameQueue& frameQueue) {

	
	int count = 0;

	int camID = 0, fps = 15, width = 1920, height = 1080, bitrate = 600000;
	std::string codec_profile = "high444";
	std::string server = "rtmp://localhost/live/stream";
	//std::string outputServer = "rtsp://localhost/mystream";
	bool dump_log = true;

	if (dump_log)
	{
		av_log_set_level(AV_LOG_DEBUG);
	}


#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(58, 9, 100)
	av_register_all();
#endif
	avformat_network_init();

	const char *output = server.c_str();
	int ret;
	//auto cam = get_device(camID, width, height);
	std::vector<uint8_t> imgbuf(height * width * 3 + 16);
	cv::Mat image(height, width, CV_8UC3, imgbuf.data(), width * 3);
	AVFormatContext *ofmt_ctx = nullptr;
	AVCodec *out_codec = nullptr;
	AVStream *out_stream = nullptr;
	AVCodecContext *out_codec_ctx = nullptr;

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
	//auto *swsctx = initialize_sample_scaler(out_codec_ctx, width, height);
	AVFrame *frame = allocate_frame_buffer(out_codec_ctx, width, height);
	//cv::namedWindow("Live", cv::WINDOW_NORMAL);
	int cur_size;
	uint8_t *cur_ptr;
	ret = avformat_write_header(ofmt_ctx, nullptr);
	if (ret < 0)
	{
		std::cout << "Could not write header!" << std::endl;
		exit(1);
	}
	AVFrame *bgrFrame = allocate_frame_buffer(out_codec_ctx, width, height);
	while (count < MAX_NUM_FRAMES) {
		if (!frameQueue.queueIsEmpty()) {

			image = frameQueue.dequeueFrame();
			
			////cv::waitKey(20);
			//////cam.read(image);
			////fr = image.clone();
			//cv::Mat fr = image;
			//int x = 0;
			//int y = 0;
			//int width = 50;
			//int height = 50;
			//// our rectangle...
			//cv::Rect rect(x, y, width, height);
			//// and its top left corner...
			//cv::Point pt1(x, y);
			//// and its bottom right corner.
			//cv::Point pt2(x + width, y + height);
			//// These two calls...
			//cv::rectangle(fr, pt1, pt2, cv::Scalar(0, 255, 0));
			//// essentially do the same thing
			//cv::rectangle(fr, rect, cv::Scalar(0, 255, 0));
			cv::Mat img_out;
			cv::cvtColor(image, img_out, cv::COLOR_BGR2YUV_YV12);
			//cv::cvtColor(image, img_out, cv::COLOR_BGR2YUV);
			//std::cout << "orig:  " << std::endl << image << std::endl;
			//std::cout << img_out << std::endl;
	
			//cv::Mat ch1, ch2, ch3;
			//// "channels" is a vector of 3 Mat arrays:
			//std::vector<cv::Mat> channels(3);
			//// split img:
			//split(img_out, channels);
			//// get the channels (dont forget they follow BGR order in OpenCV)
			//ch1 = channels[0];
			//ch2 = channels[1];
			//resize(ch2, ch2, cv::Size(ch2.cols / 2, ch2.rows));
			//ch3 = channels[2];
			//resize(ch3, ch3, cv::Size(ch3.cols / 2, ch3.rows));
			//frame->data[0] = ch1.data;
			//frame->data[1] = ch3.data;
			//frame->data[2] = ch2.data;
			
		
			cv::Mat Y = img_out(cv::Rect(0, 0, image.cols, image.rows));
			cv::Mat U = img_out(cv::Rect(0, image.rows, image.cols, image.rows / 4));
			U = U.reshape(1, image.rows / 2);
			resize(U, U, cv::Size(image.cols/2, image.rows));
			//U = U.reshape(1, image.rows / 4);
			cv::Mat V = img_out(cv::Rect(0, image.rows + image.rows / 4, image.cols, image.rows / 4));
			V = V.reshape(1, image.rows / 2);
			resize(V, V, cv::Size(image.cols/2, image.rows));
			//img_out(cv::Rect(0, image.rows, image.cols, image.rows / 4)) = U;
			frame->data[0] = Y.data;
			frame->data[1] = U.data;
			frame->data[2] = V.data;


			const int stride[] = { static_cast<int>(image.step[0]) };

			int linesize[8] = { 640,0,0, 0, 0, 0, 0, 0 };
			linesize[0] = *stride;
			//av_image_fill_arrays(frame->data, linesize, img_out.data, out_codec_ctx->pix_fmt, width, height, 0);
			
			//sws_scale(swsctx, (uint8_t const * const *)bgrFrame->data, stride, 0, image.rows, frame->data, frame->linesize);

			//av_image_fill_arrays(bgrFrame->data, bgrFrame->linesize, image.data, out_codec_ctx->pix_fmt, width, height, 0);
			//sws_scale(swsctx, (uint8_t const * const *)bgrFrame->data, stride, 0, image.rows, frame->data, frame->linesize);

			//sws_scale(swsctx, (uint8_t const * const *)&image.data, stride, 0, image.rows, frame->data, frame->linesize);


			frame->pts += av_rescale_q(1, out_codec_ctx->time_base, out_stream->time_base);
			write_frame(out_codec_ctx, ofmt_ctx, frame);
			//std::cout << "hello" << std::endl;
			count++;
		}
	}

	av_write_trailer(ofmt_ctx);

	av_frame_free(&frame);
	avcodec_close(out_codec_ctx);
	avio_close(ofmt_ctx->pb);
	avformat_free_context(ofmt_ctx);

}