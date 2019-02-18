#ifndef REST_SERVICE_HPP__
#define REST_SERVICE_HPP__

#include <string>
#include <vector>

namespace network { class Router; }

namespace RestService {

enum Operation { POST, GET };

void SetTransportLayer( network::Router* router );

std::string Send( Operation op, std::string const& url,
  std::vector<std::pair<std::string, std::string > > headers,
  std::string const& payload );


}

#endif // REST_SERVICE_HPP__
