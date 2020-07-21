//g++ -w main.cpp modules/frameManager/frameManager.cpp modules/decoder/decoder.cpp modules/display/display.cpp -o main -std=c++0x -pthread -lX11 $(pkg-config --cflags --libs libavformat libswscale libavcodec libavutil opencv4)
#include <opencv2/opencv.hpp>
#include <thread>
#include <X11/Xlib.h>
#include "modules/decoder/decoder.h"
#include "modules/display/display.h"
#include "modules/frameManager/frameManager.h"

int main()
{

    FrameManager m1(1, 30);
    std::deque<cv::Mat>myList;
    XInitThreads();
	int max = 1000;
    std::thread decoder1(decoder(),&m1);
	std::thread display1(display(), &m1);

    decoder1.join();
    display1.join();

	return 0;
}