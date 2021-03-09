/**
    Intelligent Surveillance System
    @file backgroundExtractor.h
    @author Tharmigan Krishnananthalingam
*/

#ifndef _BACKGROUNDEXTRACTOR_H
#define _BACKGROUNDEXTRACTOR_H

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/video.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/bgsegm.hpp>
#include <opencv2/core/cuda.hpp>
#include <opencv2/cudaarithm.hpp>
#include <opencv2/cudabgsegm.hpp>
#include "../videoStream/videoStream.h"

class BackgroundExtractor {

    cv::Mat fgMask;
    cv::Mat background;
    cv::Ptr<cv::BackgroundSubtractor> background_subtractor;
    cv::cuda::GpuMat frame_gpu;
    cv::cuda::GpuMat fgMask_gpu;
    cv::cuda::GpuMat background_gpu;
    bool gpu;
    int delay;

    BackgroundExtractor(bool gpu, int delay);

    cv::Mat getBackground();

    cv::Mat getForgroundMask();

    // void operator()(VideoStream& videoStream);

    public :
    
        void operator()(VideoStream& videoStream, cv::Mat& frame);

};

#endif