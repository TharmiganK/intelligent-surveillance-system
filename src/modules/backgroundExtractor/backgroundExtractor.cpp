/**
    Intelligent Surveillance System
    @file backgroundExtractor.cpp
    @author Tharmigan Krishnananthalingam
*/

#include "backgroundExtractor.h"
#define MAX_NUM_FRAMES 5000

BackgroundExtractor::BackgroundExtractor (bool gpu, int delay):
    gpu(gpu),
    delay(delay)
    {
        if (gpu){
            background_subtractor = cv::cuda::createBackgroundSubtractorMOG2(120, 250.,true);
        }
        else{
            background_subtractor = cv::createBackgroundSubtractorMOG2(120, 250.,true);
        }
    }

cv::Mat BackgroundExtractor::getBackground(){
    return background;
}

cv::Mat BackgroundExtractor::getForgroundMask(){
    return fgMask;
}

void BackgroundExtractor::operator()(VideoStream& videoStream){
    int count = 0;
    while (count < MAX_NUM_FRAMES) {
        if (!videoStream.frameQueue.queueIsEmpty()) {

            auto frame = videoStream.frameQueue.dequeueFrame();

            if (gpu){
                background_subtractor -> apply(frame, fgMask);
                background_subtractor -> getBackgroundImage(background);
            }

            else{
                frame_gpu.upload(frame);
                background_subtractor -> apply(frame_gpu, fgMask_gpu);
                background_subtractor -> getBackgroundImage(background_gpu);
                fgMask_gpu.download(fgMask);
                background_gpu.download(background);
            }
            
			count++;
		}
	}
}