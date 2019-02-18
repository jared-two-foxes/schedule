
#include <schedule/api/current_rms/current_rms.hpp>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace current_rms {
  const char kSubdomain[] = "twofoxesstyling";
  const char kOpportunitiesUri[] = "https://api.current-rms.com/api/v1/opportunities";
  const char kAuthToken[] = "xKC4nNALzixkvovsqmuG";
}

typedef std::pair<std::string, std::string > StringPair;


std::string setupUrlWithParams( std::string const & uri,
  std::vector<StringPair > params )
{
    std::string url = uri;

    int32_t i = 0;
    for ( auto& pair : params )
    {
        url += ( i == 0 ? "?" : "&" );
        url += pair.first;
        if( pair.second.length() > 0 )
        {
            url += std::string("=") + pair.second;
        }
        i++;
    }

    return url;
}

void current_rms::Opportunities_GET( std::size_t page, std::size_t count,
  std::string* uri, std::vector<std::pair<std::string, std::string > >* options,
  std::string* body )
{
	std::vector<StringPair >  params;
	params.push_back( std::make_pair( "filtermode", "live" ) );
	params.push_back( std::make_pair( "view_id", "1" ) );
	params.push_back( std::make_pair( "per_page", std::to_string( count ) ) );
	params.push_back( std::make_pair( "page", std::to_string( page ) ) );

	*uri = setupUrlWithParams( current_rms::kOpportunitiesUri, params );

	*options =
  {
    {"X-SUBDOMAIN", current_rms::kSubdomain},
    {"X-AUTH-TOKEN", current_rms::kAuthToken}
  };

	*body = std::string();
}
