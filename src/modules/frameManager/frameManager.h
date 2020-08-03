/**
    Intelligent Surveillance System
    @file frameManager.h
    @author Lavinan Selvaratnam
*/

#ifndef _FRAMEMANAGER_H
#define _FRAMEMANAGER_H
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <boost/log/trivial.hpp>
#include <boost/circular_buffer.hpp>
#include <mutex>

/**
    @class Class to manage the frame queue
    @details FrameManager has a thread safety queue to store the decoded frames temporarily until
    it is accessed. When queue is full it drops the first frame before adding a new one.
*/
class FrameManager {

    private:

        int streamID; /*!< ID of the associated video stream */
        boost::circular_buffer<cv::Mat> frameQueue; /*!< A circular buffer queue to keep the incoming frames */
        int queueCapacity; /*!< Maximum number of frames that can be present inside the queue */
        std::mutex mutexForQueue; /*!< Mutex to give exclusive access to the queue */

    public:

        /**
            @brief Constructor of class FrameManager.
            @param streamID ID of the associated camera stream.
            @param queueCapacity maximum number of frames that can exist in the queue.
        */
        FrameManager(int streamID, int queueCapacity);

        /**
            @brief To add a frame at the end of queue.
            @param newFrame The frame that should be added.
        */
        void enqueueFrame(cv::Mat newFrame);

        /**
            @brief Take out the first frame from the queue.
            @return The frame which was taken from the queue.
        */
        cv::Mat dequeueFrame();

        /**
            @brief Check the numer of frames present in the queue.
            @return number of frames that is present in the queue.
        */
        int queueLength();

        /**
            @brief Check whether there is any frame present in the queue.
            @return true if there is no frame and false if there is any frame persent.
        */
        bool queueIsEmpty();

    private:

        /**
            @brief Drop the first frame in the queue
        */
        void dropFrame();

};

#endif