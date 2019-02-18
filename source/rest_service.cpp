
#include <schedule/rest_service.hpp>
#include <network/transport/router.hpp>
#include <network/transport/request.hpp>
#include <network/transport/response.hpp>

#include <assert.h>

static network::Router* s_router = nullptr;

void RestService::SetTransportLayer( network::Router* router )
{
  s_router = router;
}

std::string RestService::Send( Operation op, std::string const& url,
  std::vector<std::pair<std::string, std::string > > headers,
  std::string const& payload )
{
  assert ( s_router );

  network::Request request;
  network::Response response;

  request.uri_ = url;
  request.options_ = headers;
  request.content_ = payload;

  auto status = s_router->perform( request, &response );
  if ( status.ok() )
  {
    return response.body_;
  }

  return std::string();
}
