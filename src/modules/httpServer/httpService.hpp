#ifndef HTTPSERVICE_HPP
#define HTTPSERVICE_HPP

#include <string>
#include <memory>
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "../streamReceiver/streamReceiver.h"
#include "../display/display.h"
#include "../decoder/decoder.h"
#include "../frameQueue/frameQueue.h"
#include "../packetQueue/packetQueue.h"
#include "../videoStream/videoStream.h"
#include "../outputStreamer/outputStreamer.h"

//http methods types
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

//each http request returned by parser
struct HttpRequest
{
  HttpMethods method;
  std::string request;
  std::string resource;
  std::string http_version;
  boost::property_tree::ptree body;
  unsigned int status;
};

class HttpRequestParser
{
  public:
    HttpRequestParser(std::string&);
    std::shared_ptr<HttpRequest> GetHttpRequest();

  private:
    std::string m_HttpRequest;
};

class HttpService
{
  public:
    HttpService(std::shared_ptr<boost::asio::ip::tcp::socket>);

    void HttpHandleRequest();
  
  private:
    std::string GetIp();
    void ProcessPostRequest(std::shared_ptr<HttpRequest> httpRequest);
    std::string GetResponseStatus();
    void SendResponse();
    void Finish();

  private:
    std::shared_ptr<boost::asio::ip::tcp::socket> m_Socket;
    boost::asio::streambuf m_Request;
    std::string m_RequestedResource;
    unsigned int m_ResponseStatusCode;
    bool m_IsResponseSent;
    std::string m_ServerOptions;
    std::string m_ContentType;

};

#endif