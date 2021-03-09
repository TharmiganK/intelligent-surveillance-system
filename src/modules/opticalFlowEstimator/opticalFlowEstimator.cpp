/**
    Intelligent Surveillance System
    @file opticalFlowEstimator.cpp
    @author Tharmigan Krishnananthalingam
*/

#include "opticalFlowEstimator.h"
// #define MAX_NUM_FRAMES 5000

OpticalFlowEstimator::OpticalFlowEstimator (bool gpu):
    gpu(gpu)
    {
        if(gpu){
            farnebackOpticalFlow = cv::cuda::FarnebackOpticalFlow::create();
        }
    }

cv::Mat OpticalFlowEstimator::getFlow(){
    return flow;
}

// void OpticalFlowEstimator::operator()(VideoStream& videoStream){
//     int count = 0;
//     cv::Mat frame0, frame1;
//     while (count < MAX_NUM_FRAMES) {
//         if (!videoStream.frameQueue.queueIsEmpty()) {

//             auto frame = videoStream.frameQueue.dequeueFrame();

//             cv::cvtColor(frame, frame0, cv::COLOR_BGR2GRAY);

//             if (frame1.empty()) {
//                 frame0.copyTo(frame1);
//             }

//             else {
//                 if (gpu){
//                     cv::cuda::GpuMat GpuImg0(frame0);
//                     cv::cuda::GpuMat GpuImg1(frame1);
                    
//                     cv::cuda::GpuMat gflow(frame0.size(), CV_32FC2);
            
//                     farnebackOpticalFlow->calc(GpuImg0, GpuImg1, gflow);

//                     gflow.download(flow);

//                 }

//                 else{
//                     cv::Mat flow_(frame0.size(), CV_32FC2);

//                     cv::calcOpticalFlowFarneback(frame0, frame1, flow, 0.5, 3, 15, 3, 5, 1.2, 0);

//                     flow_.copyTo(flow);
//                 }

//                 cv::Mat flow_parts[2];
//                 split(flow, flow_parts);

//                 cv::cartToPolar(flow_parts[0], flow_parts[1], magnitude, angle, true);
//                 cv::normalize(magnitude, magn_norm, 0.0f, 1.0f, cv::NORM_MINMAX);
//                 angle *= ((1.f / 360.f) * (180.f / 255.f));

//                 // Build hsv image
//                 cv::Mat _hsv[3], hsv, hsv8;
//                 _hsv[0] = angle;
//                 _hsv[1] = cv::Mat::ones(angle.size(), CV_32F);
//                 _hsv[2] = magn_norm;
//                 cv::merge(_hsv, 3, hsv);
//                 hsv.convertTo(hsv8, CV_8U, 255.0);
    
//                 // Save the results
//                 cv::cvtColor(hsv8, optical_flow_image, cv::COLOR_HSV2BGR);
//             }   
            
// 			count++;
// 		}
// 	}
// }

void OpticalFlowEstimator::operator()(VideoStream& videoStream, cv::Mat& frame){
    cv::cvtColor(frame, frame0, cv::COLOR_BGR2GRAY);

    if (frame1.empty()) {
        frame0.copyTo(frame1);
    }

    else {
        if (gpu){
            cv::cuda::GpuMat GpuImg0(frame0);
            cv::cuda::GpuMat GpuImg1(frame1);
                    
            cv::cuda::GpuMat gflow(frame0.size(), CV_32FC2);
            
            farnebackOpticalFlow->calc(GpuImg0, GpuImg1, gflow);

            gflow.download(flow);

        }

        else{
            cv::Mat flow_(frame0.size(), CV_32FC2);

            cv::calcOpticalFlowFarneback(frame0, frame1, flow, 0.5, 3, 15, 3, 5, 1.2, 0);

            flow_.copyTo(flow);
        }

        cv::Mat flow_parts[2];
        split(flow, flow_parts);

        cv::cartToPolar(flow_parts[0], flow_parts[1], magnitude, angle, true);
        cv::normalize(magnitude, magn_norm, 0.0f, 1.0f, cv::NORM_MINMAX);
        angle *= ((1.f / 360.f) * (180.f / 255.f));

        // Build hsv image
        cv::Mat _hsv[3], hsv, hsv8;
        _hsv[0] = angle;
        _hsv[1] = cv::Mat::ones(angle.size(), CV_32F);
        _hsv[2] = magn_norm;
        cv::merge(_hsv, 3, hsv);
        hsv.convertTo(hsv8, CV_8U, 255.0);
    
            // Save the results
        cv::cvtColor(hsv8, optical_flow_image, cv::COLOR_HSV2BGR);
    }   
}