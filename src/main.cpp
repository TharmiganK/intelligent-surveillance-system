/**
    Intelligent Surveillance System
    @file main.cpp
    @author Lavinan Selvaratnam
*/

/**
    @todo Create makefile
    @todo Create configs class
 */

#include <iostream>
#include <opencv2/opencv.hpp>
#include <thread>
#include <X11/Xlib.h>
#include <boost/log/trivial.hpp>
#include "modules/streamReceiver/streamReceiver.h"
#include "modules/display/display.h"
#include "modules/frameManager/frameManager.h"
#include "modules/packetManager/packetManager.h"
#include "modules/videoStream/videoStream.h"

/**
    @brief Main function
    @details Temporary main function to instantiate Stream Receiver, Display, Frame and Packet Queues
    for a single stream and start receiving and displaying frames as a seperate thread.
    Compressed frames (called as packets here) are received by Stream Receiver and stored in a queue 
    with Packet Manager. Where decoder consumes the packets, decodes them and stores the decoded frames
    in another queue with Frame Manager. Display consumes the decoded frames and displays them.
*/
int main() {

    //Creating managers to manage packets and frames
    PacketManager packetManager1(1, 30);
    FrameManager frameManager1(1, 30);
    VideoStream v1(1,"googlegoogle");
    VideoStream v2(1,"googlegooe");
    //Creating seperate threads to receive the frames and display
    XInitThreads();
    std::thread streamReceiver1(StreamReceiver(),v1.packetManager,v1.frameManager);
    std::thread streamReceiver2(StreamReceiver(),v2.packetManager,v2.frameManager);
	std::thread display1(display(), v1.frameManager);
    std::thread display2(display(), v2.frameManager);

    //Waiting until the processes are over
    streamReceiver1.join();
    display1.join();

	return 0;

}