/**
    Intelligent Surveillance System
    @file httpServer.h
    @author Tharmigan Krishnananthalingam
*/

#ifndef _HTTPSERVER_HPP
#define _HTTPSERVER_HPP

#include <string>
#include <memory>
#include <atomic>
#include <thread>
#include <boost/asio.hpp>

#include "httpAcceptor.h"

/**
    @class Class to run a HTTP server
    @details HttpServer class can be used to initiate and stop a HTTP server.
    This class is created using the Boost/ASIO library. And the Server can be 
    started in a specified port with the defind number of listening threads.
*/
class HttpServer{

    private:
    
        boost::asio::io_service m_IOService; /*!< IOService to connect sockets */
        std::unique_ptr<boost::asio::io_service::work> m_Work; /*!< Defines a service work to be done */
        std::unique_ptr<HttpAcceptor> m_Acceptor;  /*!< HTTP Acceptor object */
        std::vector<std::unique_ptr<std::thread>> m_ThreadPool;  /*!< Listening threads vector */

    public:

        /**
            @brief Constructor of class HttpServer.
        */
        HttpServer();

        /**
            @brief Starting the server in the specified port with threads.
            @param port port number specified to start the server.
            @param thread_pool_size number of listening threads to be created.
        */
        void Start(unsigned short port, unsigned int thread_pool_size);

        /**
            @brief Stopping the HttpServer
        */
        void Stop();

};

#endif
