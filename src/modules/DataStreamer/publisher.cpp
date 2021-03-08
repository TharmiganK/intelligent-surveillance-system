#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <zmq.hpp>

int main () {
    //  Prepare our context and publisher
    zmq::context_t context(1);
    zmq::socket_t publisher(context, ZMQ_PUB);
    publisher.bind("tcp://*:5563");

    //std::string video_path = "http://192.168.1.3:8080/video";
    cv::VideoCapture capture("../../../../videos/TownCentreXVID.avi");
    cv::Mat frame;

    while (1) {
        // cv::Mat image = cv::imread("./wallpaper.jpg", CV_LOAD_IMAGE_GRAYSCALE );
        capture.read(frame);

        int height = frame.rows;
        int width = frame.cols;
        std::cout << "Height : " << height << " , Width : " << width << std::endl;

        
        //  Write two messages, each with an envelope and content
        //  s_sendmore (publisher, "A");
        //  s_send (publisher, "We don't want to see this");
        //  s_sendmore (publisher, "B");
        //  s_send (publisher, "We would like to see this");
        // publisher.send("B", 2, ZMQ_SNDMORE);
        // publisher.send("HelloWorld", 11, ZMQ_NOBLOCK);
        int frame_size = 3*height*width*sizeof(uint8_t);
        std::cout << "Input image size : " << frame_size << std::endl;
        std::string h_s = std::to_string(height);
        std::string w_s = std::to_string(width);
        const char* h = h_s.c_str();
        const char* w = w_s.c_str();
        std::string data_s = "Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!Helloworld!";
        const char* data = data_s.c_str(); 
        publisher.send(h, strlen(h)+1, ZMQ_SNDMORE);
        publisher.send(w, strlen(w)+1, ZMQ_SNDMORE);
        publisher.send(data, strlen(data)+1, ZMQ_SNDMORE);
        publisher.send(frame.data, frame_size, ZMQ_NOBLOCK);
        
        cv::namedWindow("The Input");
        cv::imshow("The Input", frame);

        int k = cv::waitKey(1);

        if (k == 'c'){
            cv::destroyAllWindows();
            break;
        }
        
    }
    return 0;
}