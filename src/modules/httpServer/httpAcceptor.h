/**
    Intelligent Surveillance System
    @file httpAcceptor.h
    @author Tharmigan Krishnananthalingam
*/

#ifndef _HTTPACCEPTOR_HPP
#define _HTTPACCEPTOR_HPP

#include <string>
#include <memory>
#include <atomic>
#include <boost/asio.hpp>

/**
    @class Class to accept a HTTP requests from sockets.
    @details HttpAcceptor class can be used to accept HTTP requests and connect
    to clients. This class is created using the Boost/ASIO library.
*/
class HttpAcceptor{

    private:

        boost::asio::io_service& m_IOService; /*!< IOService to connect sockets */
        boost::asio::ip::tcp::acceptor m_Acceptor; /*!< TCP/IP request acceptor */
        std::atomic<bool> m_IsStopped; /*!< Boolean value to indicate whether acceptor is running or not */

    public:

        /**
            @brief Constructor of the class HttpAcceptor.
            @param ios initiated IO Service.
            @param port_num port number of the HTTP server.
        */
        HttpAcceptor(boost::asio::io_service& ios, unsigned short port_num);

        /**
            @brief Starting the HttpAcceptor
        */
        void Start();

        /**
            @brief Stopping the HttpAcceptor
        */
        void Stop();

    private:
        
        /**
            @brief accept a async connection with a client
        */
        void AcceptConnection();
  
};

#endif
