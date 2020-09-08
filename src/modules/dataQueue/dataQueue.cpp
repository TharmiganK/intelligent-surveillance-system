/**
    Intelligent Surveillance System
    @file dataQueue.cpp
    @author Lavinan Selvaratnam & Tharmigan Krishnananthalingam
*/

/**
    @todo Abstract PacketQueue and DataQueue as a generic queue class
    @todo unit test
*/

#include "dataQueue.h"

/**
    @details Constructor of DataQueue to define the camera stream ID associated and to
    define the number of datas that should be allowed to be present in the queue.
    Memory for the queue is also allocated with a size of queueCapacity.
*/
DataQueue::DataQueue(int queueCapacity) :

    queueCapacity(queueCapacity),
    queue(queueCapacity)

    {

    }

/**
    @details Adds the data passed as argument at the end of the data queue.
    If the queue is full it drops a data and then add the new one.
    @sa dropData()
*/
bool DataQueue::enqueueData(cv::Mat newData) {

    if(DataQueue::queueLength() < DataQueue::queueCapacity) {

        std::lock_guard<std::mutex> guard(mutexForQueue);
        queue.push_back(newData);

    } else {

        if (DataQueue::dropData()) {

            std::lock_guard<std::mutex> guard(mutexForQueue);
            queue.push_back(newData);

        } else {

            BOOST_LOG_TRIVIAL(error) << "Data queue doesn't have space";
            return false;

        }

    }

    return true;
            
}

/**
    @details Takes the first data out of the queue and returns it
*/
cv::Mat DataQueue::dequeueData() {

    std::lock_guard<std::mutex> guard(mutexForQueue);
    currentData = queue.front();
    queue.pop_front();
    return currentData;

}

/**
    @details Checks the number of datas currently present inside the queue
*/
int DataQueue::queueLength() {

    std::lock_guard<std::mutex> guard(mutexForQueue);
    return queue.size();

}

/**
    @details Checks whether there is any data present inside the queue
*/
bool DataQueue::queueIsEmpty() {

    std::lock_guard<std::mutex> guard(mutexForQueue);
    return queue.empty();

}

/**
    @details Drops the first data inside the queue. This is used to make space
    before adding a new one when the queue is full
    @sa enqueueData()
    @todo change the logic to drop a data based on difference
*/
bool DataQueue::dropData() {

    if(!queueIsEmpty()) {

        std::lock_guard<std::mutex> guard(mutexForQueue);
        queue.pop_front();
        BOOST_LOG_TRIVIAL(info) << "A data is dropped";

        return true;

    }

    return false;

}
