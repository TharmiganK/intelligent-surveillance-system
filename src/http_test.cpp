#include <iostream>
#include <thread>
#include <boost/asio.hpp>

#include "modules/httpServer/httpServer.h"

// std::string RESOURCE_DIRECTORY_PATH;

int main(int argc, char** argv)
{
  unsigned short port = 1234;

  try{
    HttpServer http_server;

    unsigned int thread_pool_size = std::thread::hardware_concurrency() * 2;

    if(thread_pool_size == 0)
      thread_pool_size = 2;

    http_server.Start(port, thread_pool_size);

    //keep alive server for 5 minutes, delete this and next line
    //if you want to keep it alive for infinite time
    std::this_thread::sleep_for(std::chrono::seconds(60 * 5));

    http_server.Stop();
  }catch(boost::system::system_error &e){
    std::cout<<"Error occured, Error code = "<<e.code() 
             <<" Message: "<<e.what();
  }

  return 0;
}


