/**
    Intelligent Surveillance System
    @file videoStream.h
    @author Tharmigan , Lavinan & Sanoojan
*/

#ifndef _VIDEOSTREAM_H
#define _VIDEOSTREAM_H

#include "../frameQueue/frameQueue.h"
#include "../packetQueue/packetQueue.h"
extern "C" {
#include <libavutil/imgutils.h>
}
#include <iostream>


/**
    @struct A video stream structure
    @details VideoStream contains all the necessary information of a particular stream along
    with PacketQueue to store the received packets and FrameQueue to store the decoded frames.
*/
struct VideoStream {

    int streamID; /*!< ID of the associated video stream */
    FrameQueue frameQueue; /*!< Queue to store the frames */
	FrameQueue fgMaskQueue; 
	FrameQueue opticalFlowQueue;
    PacketQueue packetQueue; /*!< Queue to store the packets */
    const char *streamURL; /*!< URL of the associated video stream */
    AVFormatContext* formatCtx; /*!< RTSP stream format context information */
    AVCodecContext* videoCodecCtx; /*!< Video codec context information */
    int videoStreamIndex; /*!< The index of the video stream in the RTSP stream */
    bool isOpen; /*!< Value to indicate whether the stream is opened or not */
    double videoFPS; /*!< Frame per second of the video stream */
    double videoBaseTime; /*!< Base time of the video stream */
    struct SwsContext *imgCvtCtx; /*!< Image convert context information (YUV -> BGR) */
    int width; /*!< Width of the decoded frame */
    int height; /*!< Height of the decoded frame */
    int bufferImgSize; /*!< Image buffer size from video codec */
    uint8_t *buffer; /*!< Buffer image used for YUV to BGR conversion */
    AVCodec* videoCodec; /*!< Video codec decoder */
	AVFormatContext *ofmt_ctx; /*!< Output format context */
	AVCodec *out_codec; /*!< Video codec encoder */
	AVStream *out_stream; /*!< Output video stream */
	AVCodecContext *out_codec_ctx;  /*!< Video codec context */
	std::string outputURL; /*!< Output stream URL */
	std::string codec_profile; /*!< codec encoder profile */
	int bitrate; /*!< Streaming bitrate*/
	AVFrame *frame; /*!< frame for writing */
	SwsContext *swsctx; /*!< Image convert context information (RGB->YUV) */


    /**
        @brief Constructor of struct VideoStream.
        @param streamID ID of the associated camera stream.
        @param streamURL URL of the associated camera stream.
    */
    VideoStream(int streamID, const char *streamURL, int queueCapacity);

    /**
        @brief Destructor of class StreamReceiver.
    */
    virtual ~VideoStream() {

            CloseStream();

    }

    /**
        @brief Open the RTSP stream.
        @return true if RTSP stream is opened sucessfully; false otherwise.
    */
    virtual bool OpenStream();

    /**
        @brief Close the RTSP stream.
        @return true if RTSP stream is closed sucessfully; false otherwise.
    */
    virtual bool CloseStream();

    private:

        /**
            @brief Open the video stream from RTSP stream.
            @return true if video stream is opened sucessfully; false otherwise.
        */
        bool OpenVideoStream();

        /**
            @brief Close the video stream from RTSP stream.
        */
        void CloseVideoStream();

		/**
			@brief Initializing output format context.
			@param Output format context to be initialized.
			@param format name.
		*/
		void initialize_avformat_context(AVFormatContext *&fctx, const char *format_name);

		/**
			@brief Initializing IO context.
			@param Output format context to be set.
			@param output URL.
		*/
		void initialize_io_context(AVFormatContext *&fctx, const char *output);

		/**
			@brief Setting codec parameters.
			@param Output format context to be set.
			@param Codec context to be set.
			@param streaming width.
			@param streaming height.
			@param Output FPS.
			@param Output bitrate.
		*/
		void set_codec_params(AVFormatContext *&fctx, AVCodecContext *&codec_ctx, double width, double height, int fps, int bitrate);
		
		/**
			@brief Initializing output stream.
			@param stream to be inialized.
			@param Codec context.
			@param Codec encoder.
			@param Codec profile (main | high10 | baseline| high422 | high444).
		*/
		void initialize_codec_stream(AVStream *&stream, AVCodecContext *&codec_ctx, AVCodec *&codec, std::string codec_profile);

		/**
			@brief initializing sws_scaler context for converting context information.
			@brief changes RGB to AV_PIX_FMT Initialized.
			@param Codec context .
			@param streaming width.
			@param streaming height.
			@return sws scaler context.
		*/
		SwsContext *initialize_sample_scaler(AVCodecContext *codec_ctx, double width, double height);

		/**
			@brief allocating memory for output frame to write.
			@param Codec context .
			@param streaming width.
			@param streaming height.
			@return initialzed frame.
		*/
		AVFrame *allocate_frame_buffer(AVCodecContext *codec_ctx, double width, double height);

		/**
			@ Initializing output format context, IO context, codec, codec context ,sws_scale context and frame .
		*/
		void initializeOutputStream();

};

#endif