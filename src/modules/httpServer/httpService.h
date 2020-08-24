/**
    Intelligent Surveillance System
    @file httpService.h
    @author Tharmigan Krishnananthalingam
*/


#ifndef _HTTPSERVICE_HPP
#define _HTTPSERVICE_HPP

#include <string>
#include <memory>
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "../videoStream/videoStream.h"
#include "../processor/processor.h"

/**
    http methods types
*/
enum class HttpMethods{

    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE,

};

/**
    each http request returned by parser
*/
struct HttpRequest{

    HttpMethods method; /*!< Http method */
    std::string request;  /*!< Http Request string */
    std::string resource;  /*!< Http Request resource path */
    std::string http_version;  /*!< Http version */
    boost::property_tree::ptree body;  /*!< Http Request body */
    unsigned int status;  /*!< Http Request status */

};

/**
    @class Class to parse a HTTP request (string -> HttpRequest object)
    @details HttpRequestParser class can be used to parse a string HTTP request
    into a HttpRequest object.
*/
class HttpRequestParser{

    private:

        std::string m_HttpRequest; /*!< Http Request string */

    public:

        /**
            @brief Constructor of HttpRequestParser.
            @param request HTTP request string.
        */
        HttpRequestParser(std::string& request);

        /**
            @brief Get the parsed HttpRequest object.
            @return Pointer of HttpRequset object.
        */
        std::shared_ptr<HttpRequest> GetHttpRequest();

};

/**
    @class Class to handle HTTP requests and services.
    @details HttpService class contains methods to handle different kind of HTTP 
    requests. And defines the services of the server.
*/
class HttpService{

    private:

        std::shared_ptr<boost::asio::ip::tcp::socket> m_Socket; /*!< TCP/IP network socket */
        boost::asio::streambuf m_Request; /*!< Http Request String */
        std::string m_RequestedResource; /*!< Http Requested resource path */
        unsigned int m_ResponseStatusCode; /*!< Http Response status */
        bool m_IsResponseSent; /*!< Boolean value to determine whether response is sent or not */
        std::string m_ServerOptions; /*!< Http server options */
        std::string m_ContentType; /*!< Http content type */

    public:

        /**
            @brief Constructor of HttpService.
            @param sock TCP/IP network socket.
        */
        HttpService(std::shared_ptr<boost::asio::ip::tcp::socket> sock);

        /**
            @brief Handles the HTTP requests.
        */
        void HttpHandleRequest();
  
    private:
        
        /**
            @brief Returns the client IP address.
            @return IP address of the client in string.
        */
        std::string GetIp();

        /**
            @brief Returns the Response status.
            @return Response status in string.
        */
        std::string GetResponseStatus();

        /**
            @brief Sends the response to the client.
        */
        void SendResponse();

        /**
            @brief Deletes the current object.
        */
        void Finish();

        /**
            @brief Processes the POST requests from client.
            @param httpRequest HttpRequest object which contains the request info.
        */
        void ProcessPostRequest(std::shared_ptr<HttpRequest> httpRequest);

        /**
            @brief Service to add streams in Processor.
            @param httpRequest HttpRequest object which contains the request info.
        */
        void addStream(std::shared_ptr<HttpRequest> httpRequest);

};

#endif