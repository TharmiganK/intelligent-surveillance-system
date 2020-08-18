/**
	Intelligent Surveillance System
	@file outputStreamer.h
	@author Sanoojan Baliah
*/
#pragma once

#include "../frameQueue/frameQueue.h"
#include "../videoStream/videoStream.h"
#include <iostream>
#include <vector>

#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <Windows.h>
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}

/**
	@class Class to take frames from frame queue and stream them.
	@details This takes frame and encode in h264 stream and stream it with rtmp
*/
class Streamer {

public:

	/**
		@brief Member function to run as a seperate thread and stream the frames.
		@param frameQueue reference to the frame queue from which frames should be taken.
	*/
	//void operator()(FrameQueue& frameQueue);
	void operator()(VideoStream& videoStream);
};

