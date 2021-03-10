/**
    Intelligent Surveillance System
    @file opticalFlowEstimator.h
    @author Tharmigan Krishnananthalingam
*/

#ifndef _OPTICALFLOWESTIMATOR_H
#define _OPTICALFLOWESTIMATOR_H

#include <opencv2/cudaimgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/video.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/cudabgsegm.hpp>
#include <opencv2/cudaobjdetect.hpp>
#include <opencv2/cudaoptflow.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/cudaarithm.hpp"
#include "../videoStream/videoStream.h"
#include <boost/log/trivial.hpp>

class OpticalFlowEstimator {

    cv::Mat frame0, frame1;
    cv::Mat flow;
    cv::Mat magnitude;
    cv::Mat angle;
    cv::Mat magn_norm;
    cv::Mat optical_flow_image;
    cv::Ptr<cv::cuda::FarnebackOpticalFlow> farnebackOpticalFlow;
    cv::cuda::GpuMat frame_gpu;
    cv::cuda::GpuMat flow_gpu;
    int count = 0;
    bool gpu;

    public :

        OpticalFlowEstimator(bool gpu);

        cv::Mat getFlow();

        // void operator()(VideoStream& videoStream);

        void operator()(VideoStream& videoStream, cv::Mat& frame);

        void compute(VideoStream& videoStream, cv::Mat frame);
};

#endif