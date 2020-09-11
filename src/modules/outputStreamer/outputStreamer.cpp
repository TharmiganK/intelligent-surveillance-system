/**
	Intelligent Surveillance System
	@file outputStreamer.cpp
	@author Sanoojan Baliah
*/


#include "outputStreamer.h"
#define MAX_NUM_FRAMES 5000

/**
	@details convering frames to packet and write them 
*/
void write_frame(AVCodecContext *codec_ctx, AVFormatContext *fmt_ctx, AVFrame *frame, int stream_index)
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

    pkt.stream_index = stream_index;

	av_interleaved_write_frame(fmt_ctx, &pkt);
	av_packet_unref(&pkt);
}

/**
	@details dequeue from the buffer, make the frame,
	adding time details to the frames
*/
void Streamer::operator()(tbb::concurrent_vector< std::shared_ptr< VideoStream >>& videoStreams){
	int count = 0;
	while (count < MAX_NUM_FRAMES) {

        for (int i = 0; i < videoStreams.size(); i++) {

		    if (!videoStreams[i]->frameQueue.queueIsEmpty()) {

                auto videoStream = videoStreams[i];

			    auto image = videoStream->frameQueue.dequeueFrame();
			    cv::Mat img_out;
			    cv::cvtColor(image, img_out, cv::COLOR_RGB2YUV);
			
		    	std::vector<cv::Mat> channels(3);
		    	split(img_out, channels);

			    videoStream->frame->data[0] = channels[0].data;
			    videoStream->frame->data[1] = channels[2].data;
			    videoStream->frame->data[2] = channels[1].data;

                videoStream->frame1->data[0] = channels[0].data;
			    videoStream->frame1->data[1] = channels[2].data;
			    videoStream->frame1->data[2] = channels[1].data;

			    /*const int stride[] = { static_cast<int>(image.step[0]) };
			    sws_scale(videoStream.swsctx, (uint8_t const * const *)&image.data, stride, 0, image.rows, videoStream.frame->data, videoStream.frame->linesize);
			    */
			    videoStreams[i]->frame->pts += av_rescale_q(1, videoStream->out_codec_ctx->time_base, videoStream->out_stream->time_base);
                videoStreams[i]->frame1->pts += av_rescale_q(1, videoStream->out_codec_ctx1->time_base, videoStream->out_stream1->time_base);
			    write_frame(videoStream->out_codec_ctx, videoStream->ofmt_ctx, videoStream->frame, videoStream->out_stream->id);
                write_frame(videoStream->out_codec_ctx1, videoStream->ofmt_ctx, videoStream->frame1, videoStream->out_stream1->id);
			    count++;
		    }

        }
	}

    for (int i = 0; i < videoStreams.size(); i++) {
        auto videoStream = videoStreams[i];
	    av_write_trailer(videoStream->ofmt_ctx);
	    //sws_freeContext(swsctx);
	    av_frame_free(&videoStream->frame);
	    avcodec_close(videoStream->out_codec_ctx);
	    avio_close(videoStream->ofmt_ctx->pb);
	    avformat_free_context(videoStream->ofmt_ctx);
    }

}