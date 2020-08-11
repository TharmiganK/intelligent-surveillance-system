/**
    Intelligent Surveillance System
    @file frameQueueTest.cpp
    @author Lavinan Selvaratnam
*/

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Suites
#include <boost/test/unit_test.hpp>
#define private public
#include "../frameQueue/frameQueue.h"

 
BOOST_AUTO_TEST_SUITE(frameQueue)
 
BOOST_AUTO_TEST_CASE(enqueueFrame) {

    FrameQueue frameQueue1(1,1);
    cv::Mat testFrame = cv::Mat::zeros(cv::Size(3840, 2160), CV_64FC1);
    BOOST_CHECK(frameQueue1.enqueueFrame(testFrame));
    BOOST_CHECK(frameQueue1.enqueueFrame(testFrame));
    FrameQueue frameQueue2(2,0);
    BOOST_CHECK(!frameQueue2.enqueueFrame(testFrame));

}

BOOST_AUTO_TEST_CASE(dequeueFrame) {

    FrameQueue frameQueue1(1,1);
    cv::Mat testFrame = cv::Mat::zeros(cv::Size(3840, 2160), CV_64FC1);
    frameQueue1.queue.push_back(testFrame);
    BOOST_CHECK(std::equal(testFrame.begin<uchar>(),
                            testFrame.end<uchar>(),
                            frameQueue1.dequeueFrame().begin<uchar>()
                            ));


}

BOOST_AUTO_TEST_CASE(queueLength) {

    FrameQueue frameQueue1(1,1);
    BOOST_CHECK(frameQueue1.queueLength() == frameQueue1.queue.size());
    cv::Mat testFrame = cv::Mat::zeros(cv::Size(3840, 2160), CV_64FC1);
    frameQueue1.queue.push_back(testFrame);
    BOOST_CHECK(frameQueue1.queueLength() == frameQueue1.queue.size());

}

BOOST_AUTO_TEST_CASE(queueIsEmpty) {

    FrameQueue frameQueue1(1,1);
    BOOST_CHECK(frameQueue1.queueIsEmpty());
    cv::Mat testFrame = cv::Mat::zeros(cv::Size(3840, 2160), CV_64FC1);
    frameQueue1.queue.push_back(testFrame);
    BOOST_CHECK(!frameQueue1.queueIsEmpty());

}

BOOST_AUTO_TEST_CASE(dropFrame) {

    FrameQueue frameQueue1(1,1);
    cv::Mat testFrame = cv::Mat::zeros(cv::Size(3840, 2160), CV_64FC1);
    frameQueue1.queue.push_back(testFrame);
    BOOST_CHECK(frameQueue1.dropFrame());
    BOOST_CHECK(!frameQueue1.dropFrame());

}
 
BOOST_AUTO_TEST_SUITE_END()
 
BOOST_AUTO_TEST_SUITE(Physics)
 
BOOST_AUTO_TEST_CASE(specialTheory)
{
    int e = 32;
    int m = 2;
    int c = 4;
 
    BOOST_CHECK(e == m * c * c);
}
 
BOOST_AUTO_TEST_SUITE_END()