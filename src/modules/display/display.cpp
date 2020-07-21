#include "display.h"



void display::operator()(FrameManager *m1) {

    // the access to this function is mutually exclusive
    cv::Mat frame2;
    int i = 0;
    while(i < 10000) {
        if (!m1->queueIsEmpty()) {
            {
            // std::lock_guard<std::mutex> guard(myMutex);
            std::cout << "Length of queue : " << m1->queueLength()<< std::endl;
            frame2 = m1->dequeueFrame();
            }
        //     cv::Mat image(codec_ctx->height, codec_ctx->width, CV_8UC3, frame2->data[0], frame2->linesize[0]);
            cv::imshow("frame", frame2);
            cv::waitKey(200);
            i = i + 1;
        }
    }
    std::cout << "DONE, Remaining frames : " << m1->queueLength()<< " Iterations : " << i << std::endl;

}