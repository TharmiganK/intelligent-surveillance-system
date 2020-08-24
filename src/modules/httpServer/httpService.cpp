/**
    Intelligent Surveillance System
    @file httpService.cpp
    @author Tharmigan Krishnananthalingam
*/

#include <fstream>
#include <atomic>
#include <thread>
#include <memory>
#include <chrono>
#include <ctime>
#include <iostream>
#include <unordered_map>
#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>
#include <boost/process.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include "httpService.h"

#define QUEUE_CAPACITY 30

using namespace boost;

/**
    HTTP status table
 */
std::unordered_map<unsigned int, std::string> HttpStatusTable =
{
  {101, "101 Switching Protocols"},
  {201, "201 Created"},
  {202, "202 Accepted"},
  {200, "200 OK" },
  {400, "400 Bad Request"},
  {401, "401 Unauthorized"},
  {404, "404 Not Found" },
  {408, "408 Request Timeout"},
  {413, "413 Request Entity Too Large" },
  {500, "500 Internal Server Error" },
  {501, "501 Not Implemented" },
  {502, "502 Bad Gateway"},
  {503, "503 Service Unavailable"},
  {505, "505 HTTP Version Not Supported" }
};

/**
    @details constructor takes the HTTP request string.
*/
HttpRequestParser::HttpRequestParser(std::string& request):
    m_HttpRequest(request)
{}

/**
    @details HTTP request parser, parses the request made by client and stores 
    it into HttpRequest structure and return it
*/
std::shared_ptr<HttpRequest> HttpRequestParser::GetHttpRequest(){

    if(m_HttpRequest.empty())  return nullptr;

    std::string request_method, resource, http_version, body;
    std::string text;
    bool bodyFound = false;
    std::istringstream request_line_stream(m_HttpRequest);

    //extract request method, GET, POST, .....
    request_line_stream >> request_method;

    //extract requested resource
    request_line_stream >> resource;

    //extract HTTP version
    request_line_stream >> http_version;

    //extract body
    while (request_line_stream){
        request_line_stream >> text;

        if (text.compare("{") == 0){
            bodyFound = true;
        }

        if (bodyFound){
            body += text;
        }

        if (text.compare("}") == 0){
            bodyFound = false;
        }
    }

    property_tree::ptree pt;

    if (!body.empty()){
        std::stringstream ss;
        ss << body;
        property_tree::read_json(ss,pt);
    }

    std::shared_ptr<HttpRequest> request(new HttpRequest);

    request->resource = std::move(resource);
    request->status = 0;
    request->body = pt;

    if(request_method.compare("GET") == 0){
        request->method = HttpMethods::GET;
    }
    else if(request_method.compare("HEAD") == 0){
        request->method = HttpMethods::HEAD;
    }
    else if(request_method.compare("POST") == 0){
        request->method = HttpMethods::POST;
    }
    else if(request_method.compare("PUT") == 0){
        request->method = HttpMethods::PUT;
    }
    else if(request_method.compare("DELETE") == 0){
        request->method = HttpMethods::DELETE;
    }
    else if(request_method.compare("CONNECT") == 0){
        request->method = HttpMethods::CONNECT;
    }
    else if(request_method.compare("OPTIONS") == 0){
        request->method = HttpMethods::OPTIONS;
    }
    else if(request_method.compare("TRACE") == 0){
        request->method = HttpMethods::TRACE;
    }
    else{
        request->status = 400;
    }

    if(http_version.compare("HTTP/1.1") == 0){
        request->http_version = "1.1";
    }else{
        request->status = 505;
    }

    request->request = std::move(m_HttpRequest);

    return request;

}

/**
    @details Constructor takes TCP/IP network socket.
*/
HttpService::HttpService(std::shared_ptr<asio::ip::tcp::socket> sock):
    m_Socket(sock),
    m_Request(4096),
    m_IsResponseSent(false)
{
}

/**
    @details Handles each HTTP request made by client by calling process methods.
*/
void HttpService::HttpHandleRequest(){

    //read the request from client
    asio::async_read_until(*m_Socket.get(),
        m_Request, '\r',
        [this](const boost::system::error_code& ec, 
            std::size_t bytes_transfered)
            {
                //get string from read stream
                std::string request_line;
                std::istream request_stream(&m_Request);
                std::getline(request_stream, request_line, '\0');

                //parse the string and get HTTP request
                HttpRequestParser parser(request_line);
                std::shared_ptr<HttpRequest> http_request = parser.GetHttpRequest();

                // std::cout<<"[ Handling Client: "<<GetIp()<<" ]"<<std::endl;
                // std::cout<<"Request: {"<<std::endl;
                // std::cout<<http_request->request;
                // std::cout<<"}"<<std::endl;

                if(http_request->status == 0){
                    m_RequestedResource = http_request->resource;
                    // std::cout << m_RequestedResource << std::endl;

                    //handle each method
                    switch(http_request->method){
                        case HttpMethods::GET :
                            BOOST_LOG_TRIVIAL(info) << "RECEIVED A GET REQUEST.";
                            m_ResponseStatusCode = 200;
                            SendResponse();
                            break;
                        case HttpMethods::POST :
                            BOOST_LOG_TRIVIAL(info) << "RECEIVED A POST REQUEST.";
                            ProcessPostRequest(http_request);
                            break;
                        case HttpMethods::HEAD :
                            BOOST_LOG_TRIVIAL(info) << "RECEIVED A HEAD REQUEST.";
                            break;
                        case HttpMethods::DELETE :
                            BOOST_LOG_TRIVIAL(info) << "RECEIVED A DELETE REQUEST.";
                            break;
                        case HttpMethods::OPTIONS :
                            BOOST_LOG_TRIVIAL(info) << "RECEIVED A OPTIONS REQUEST.";
                            break;
                        default: break;
                    }
                }
                else{
                    m_ResponseStatusCode = http_request->status;
                    if(!m_IsResponseSent)
                        SendResponse();
                    return;
                }
            });
}

/**
    @details Returns ip address of connected endpoint from the network socket.
*/
std::string HttpService::GetIp(){

    asio::ip::tcp::endpoint ep = m_Socket->remote_endpoint();
    asio::ip::address addr = ep.address();
    return std::move(addr.to_string());

}

/**
    @details Processes all the POST methods from the RequestHandler. 
    And redirects the requests corresponding to the request resource path. 
*/
void HttpService::ProcessPostRequest(std::shared_ptr<HttpRequest> httpRequest){

    if (httpRequest->resource.compare("/addStream")){
        addStream(httpRequest);
    }
    else{
        m_ResponseStatusCode = 400;
        SendResponse();
    }

}

/**
    @details Processes the add stream requests from the client. Extracts the 
    stream path and stream index from the body and creates video stream objects 
    in the processor.
*/
void HttpService::addStream(std::shared_ptr<HttpRequest> httpRequest){

    std::string url;
    int streamID;

    try{
        url = httpRequest->body.get<std::string>("url");
        const char* streamURL = url.c_str();
        streamID = httpRequest->body.get<int>("id");
        // std::cout << "URL : " << url << std::endl;
        // std::cout << "Stream ID : " << streamID << std::endl;

        Processor* processor = new Processor();
        processor->addStream(new VideoStream(streamID, streamURL, QUEUE_CAPACITY));
        m_ResponseStatusCode = 201;
        SendResponse();

        processor->process();
        processor->join();
    }
    catch(std::exception const& e){
        std::cerr<<e.what()<<std::endl;
        m_ResponseStatusCode = 400;
        SendResponse();
        return;
    }
}

/**
    @details Creates the HTTP response string.
*/
std::string HttpService::GetResponseStatus(){

    std::string response_status;

    auto end = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    std::string timestr(std::ctime(&end_time));

    m_Socket->shutdown(asio::ip::tcp::socket::shutdown_receive);

    auto status_line = HttpStatusTable[m_ResponseStatusCode];

    response_status = std::string("HTTP/1.1 ") + status_line + "\n";

    if(!m_ContentType.empty()){
        response_status += m_ContentType + "\n";
    }
    else{
        response_status += std::string("Content-Type: text/html") + "\n";
    }

    if(!m_ServerOptions.empty()){
        response_status += std::string("Allow: ") + std::move(m_ServerOptions) + "\n";
    }

    response_status += std::string("Server: TinyHttpWebServer/0.0.1") + "\n";
    response_status += std::string("AcceptRanges: bytes") + "\n";
    response_status += std::string("Connection: Closed") + "\n";
    response_status += std::string("Date: ") + timestr + "\n";

    return std::move(response_status);

}

/**
    @details Deletes the current object and all the data, once response is sent.
*/
void HttpService::Finish(){

    delete this;

}

/**
    @details Sends the HTTP response by writing it in the socket.
*/
void HttpService::SendResponse(){

    std::vector<asio::const_buffer> response_buffers;

    m_IsResponseSent = true;

    std::string response_status = GetResponseStatus();
    response_buffers.push_back(asio::buffer(std::move(response_status)));

    //send response to client with data
    asio::async_write(*m_Socket.get(),
        response_buffers,
        [this](const boost::system::error_code& ec,
            std::size_t bytes_transferred)
            {
                if(ec){
                    std::cout<<"Error occured, Error code = "<<ec.value()
                        <<" Message: "<<ec.message();
                }

                Finish();
            });

}
