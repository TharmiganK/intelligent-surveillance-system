/**
    Intelligent Surveillance System
    @file packetQueueTest.cpp
    @author Lavinan Selvaratnam
*/

#define private public

#include <boost/test/unit_test.hpp>
#include "../packetQueue/packetQueue.h"
 
BOOST_AUTO_TEST_SUITE(packetQueue)
 
BOOST_AUTO_TEST_CASE(enqueuePacket) {

    PacketQueue packetQueue1(1,1);
    AVPacket testPacket;
    av_init_packet(&testPacket);
    BOOST_CHECK(packetQueue1.enqueuePacket(testPacket));
    BOOST_CHECK(packetQueue1.enqueuePacket(testPacket));
    PacketQueue packetQueue2(2,0);
    BOOST_CHECK(!packetQueue2.enqueuePacket(testPacket));

}

BOOST_AUTO_TEST_CASE(dequeuePacket) {

    PacketQueue packetQueue1(1,1);
    AVPacket testPacket;
    av_init_packet(&testPacket);
    packetQueue1.queue.push_back(testPacket);
    BOOST_CHECK(packetQueue1.dequeuePacket().data == testPacket.data);

}

BOOST_AUTO_TEST_CASE(queueLength) {

    PacketQueue packetQueue1(1,1);
    BOOST_CHECK(packetQueue1.queueLength() == packetQueue1.queue.size());
    AVPacket testPacket;
    av_init_packet(&testPacket);
    packetQueue1.queue.push_back(testPacket);
    BOOST_CHECK(packetQueue1.queueLength() == packetQueue1.queue.size());

}

BOOST_AUTO_TEST_CASE(queueIsEmpty) {

    PacketQueue packetQueue1(1,1);
    BOOST_CHECK(packetQueue1.queueIsEmpty());
    AVPacket testPacket;
    av_init_packet(&testPacket);
    packetQueue1.queue.push_back(testPacket);
    BOOST_CHECK(!packetQueue1.queueIsEmpty());

}

BOOST_AUTO_TEST_CASE(dropPacket) {

    PacketQueue packetQueue1(1,1);
    AVPacket testPacket;
    av_init_packet(&testPacket);
    packetQueue1.queue.push_back(testPacket);
    BOOST_CHECK(packetQueue1.dropPacket());
    BOOST_CHECK(!packetQueue1.dropPacket());

}
 
BOOST_AUTO_TEST_SUITE_END()