
#include <schedule/rest_service.hpp>
#include <network/transport/router.hpp>
#include <network/transport/request.hpp>
#include <network/transport/response.hpp>

#include <assert.h>

using network::Router;
using network::Response;
using network::Request;
using network::HttpMethod;


static Router* s_router = nullptr;

void RestService::SetTransportLayer( Router* router )
{
  s_router = router;
}


std::string Process( Request const & request, Response * response )
{
  auto status = s_router->perform( request, response );
  if ( status.ok() )
  {
    return response->body_;
  }

  return std::string();
}

std::string RestService::Post( std::string const& url,
  std::vector<std::pair<std::string, std::string > > headers,
  std::string const& payload )
{
  assert ( s_router );

  Request request;
  Response response;

  request.method_ = HttpMethod::POST;
  request.uri_ = url;
  request.options_ = headers;
  request.content_ = payload;

  return Process( request, &response );
}

std::string RestService::Get( std::string const& url,
  std::vector<std::pair<std::string, std::string > > headers,
  std::string const& payload )
{
  assert ( s_router );

  Request request;
  Response response;

  request.method_ = HttpMethod::GET;
  request.uri_ = url;
  request.options_ = headers;
  request.content_ = payload;

  return Process( request, &response );
}