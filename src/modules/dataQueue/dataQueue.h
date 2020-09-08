/**
    Intelligent Surveillance System
    @file dataQueue.h
    @author Lavinan Selvaratnam & Tharmigan Krishnananthalingam
*/

#ifndef _DATAQUEUE_H
#define _DATAQUEUE_H
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <boost/log/trivial.hpp>
#include <boost/circular_buffer.hpp>
#include <mutex>

/**
    @class Class to manage the data queue
    @details DataQueue has a thread safety queue to store the decoded data temporarily until
    it is accessed. When queue is full it drops the first data before adding a new one.
*/
class DataQueue {

    private:

        boost::circular_buffer<cv::Mat> queue; /*!< A circular buffer queue to keep the incoming data */
        int queueCapacity; /*!< Maximum number of data that can be present inside the queue */
        std::mutex mutexForQueue; /*!< Mutex to give exclusive access to the queue */
        cv::Mat currentData; /*!< Data which is currently consumed from the queue to process */

    public:

        /**
            @brief Constructor of class DataQueue.
            @param streamID ID of the associated camera stream.
            @param queueCapacity Number of data that can exist in the queue.
        */
        DataQueue(int queueCapacity);

        /**
            @brief To add a data at the end of queue.
            @param newData The data that should be added.
        */
        bool enqueueData(cv::Mat newData);

        /**
            @brief Take out the first data from the queue.
            @return The data which was taken from the queue.
        */
        cv::Mat dequeueData();

        /**
            @brief Check the numer of data present in the queue.
            @return number of data that is present in the queue.
        */
        int queueLength();

        /**
            @brief Check whether there is any data present in the queue.
            @return true if there is no data and false if there is any data persent.
        */
        bool queueIsEmpty();

    private:

        /**
            @brief Drop the first data in the queue
        */
        bool dropData();

};

#endif