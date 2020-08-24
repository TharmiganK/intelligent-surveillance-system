/**
    Intelligent Surveillance System
    @file httpServer.cpp
    @author Tharmigan Krishnananthalingam
*/

#include <atomic>
#include <thread>
#include <memory>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>

#include "httpServer.hpp"

using namespace boost;

/**
    @details constructor creates a new work for IO service.
*/
HttpServer::HttpServer(){

    m_Work.reset(new asio::io_service::work(m_IOService));

}

/**
    @details Start method initialize a HTTP server on the specified port and 
    creates given number of threads for listening. 
*/
void HttpServer::Start(unsigned short port, unsigned int thread_pool_size){

    if(thread_pool_size <= 0)  return;

    //create and start HttpAcceptor for accepting connection requests
    m_Acceptor.reset(new HttpAcceptor(m_IOService, port));
    m_Acceptor->Start();

    BOOST_LOG_TRIVIAL(info) << "HTTP SERVER STARTED AT ADDRESS: 127.0.0.1, PORT :1234";
    BOOST_LOG_TRIVIAL(info) << "GOTO http://127.0.0.1:1234/";

    //create specified number of threads and add them to the pool
    for(unsigned int i = 0; i < thread_pool_size; i++){
        std::unique_ptr<std::thread> 
            th(new std::thread([this]()
                {
                    //run the socket io service
                    m_IOService.run();
                })
            );

        m_ThreadPool.push_back(std::move(th));
    }

}

/**
    @details Stop method stops the IO Service and Rquest Acceptor and join all 
    the running threads to finish.
*/
void HttpServer::Stop(){
    // m_Acceptor->Stop();
    // m_IOService.stop();

    for(auto& th : m_ThreadPool){
        th->join();
    }

}


