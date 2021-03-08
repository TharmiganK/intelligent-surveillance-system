#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
//#include "zhelpers.hpp"
#include <zmq.hpp>


int main () {
    //  Prepare our context and subscriber

    zmq::context_t context(1);
    zmq::socket_t subscriber (context, ZMQ_SUB);
    subscriber.connect("tcp://localhost:5563");
    subscriber.setsockopt( ZMQ_SUBSCRIBE, "", 0);

    while (1) {
 
		//  Read envelope with address
		// std::string address = s_recv (subscriber);
        // zmq::message_t message1;
        // subscriber.recv(&message1, ZMQ_RCVMORE);
        // std::string address = std::string(static_cast<char*>(message1.data()), message1.size());
		//  Read message contents
		// std::string contents = s_recv (subscriber);
        zmq::message_t message1;
        subscriber.recv(&message1);
        char* height = (char*) message1.data();

        zmq::message_t message2;
        subscriber.recv(&message2, ZMQ_RCVMORE);
        char* width = (char*) message2.data();

        zmq::message_t message3;
        subscriber.recv(&message3, ZMQ_RCVMORE);
        char* data = (char*) message3.data();

        zmq::message_t message;
        subscriber.recv(&message);
        char* image_data = (char*) message.data();

        // char* image_data[2073600];
        // subscriber.recv(image_data, 2073600, 0);
		
        cv::Mat image(1080,1920, CV_8UC3, image_data);

       /* cv::namedWindow("The Output");
        cv::imshow("The Output", image);*/
        int k = cv::waitKey(1);

        if (k == 'c'){
            cv::destroyAllWindows();
            break;
        }

        std::cout << "Height : " << height << ", Width : " << width << std::endl;
        std::cout << "Data : " << data << std::endl;
        // std::cout << "[" << height << "] " << image_data << std::endl;
    }
    return 0;
}