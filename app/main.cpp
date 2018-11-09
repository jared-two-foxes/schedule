
#include <schedule/api/opportunity.hpp>
#include <schedule/auth/authenticate.hpp>
#include <network/NetworkManager.hpp>
#include <foundation/base/functional.hpp>
#include <foundation/strings/strcat.hpp>

#include <rapidjson/document.h>

#include <assert.h>
#include <iostream>
#include <string>
#include <vector>

using namespace schedule;
using namespace foundation;

namespace currentrms {
  const char kSubdomain[] = "twofoxesstyling";
  const char kOpportunitiesUri[] = "https://api.current-rms.com/api/v1/opportunities";
  const char kAuthToken[] = "xKC4nNALzixkvovsqmuG";
}

namespace googleapis {
  const char kClientId[] = "64255872448-1grg1talmvfeui14s4ddh6jhade90l3q.apps.googleusercontent.com";
  const char kClientSecret[] = "rzlhuPrFOwNxXMbC_Ed8AK_L";
  const char kDefaultAuthUri[] = "https://accounts.google.com/o/oauth2/auth";
  const char kDefaultTokenUri[] = "https://accounts.google.com/o/oauth2/token";
  const char kDefaultRevokeUri[] = "https://accounts.google.com/o/oauth2/revoke";
  const char kOutOfBandUrl[] = "urn:ietf:wg:oauth:2.0:oob";

  const char kCalendarScope[] = "https://www.googleapis.com/auth/calendar";
}


bool retrieveOpportunities( std::string& readBuffer,
        std::vector<current_rms::opportunity >* opportunities )
{
    rapidjson::Document document;
    document.Parse( readBuffer.c_str(), readBuffer.length() );

    assert( document.HasMember("opportunities") );
    assert( document["opportunities"].IsArray() );
    if ( !document["opportunities"].IsNull() )
    {
        const auto& ar = document["opportunities"].GetArray();
        for ( auto& a : ar )
        {
            opportunities->push_back( current_rms::convert( a ) );
        }

        return ar.Size();
    }

    return -1;
}

void populateFromServer( network::NetworkManager* mgr,
        std::vector<current_rms::opportunity >* opportunities )
{
    int32_t page = 1;

    // Set up transport layer options.
    network::ParameterList optsList;
    optsList.push_back(std::make_pair("X-SUBDOMAIN", currentrms::kSubdomain ));
    optsList.push_back(std::make_pair("X-AUTH-TOKEN", currentrms::kAuthToken ));

    network::Request request( currentrms::kOpportunitiesUri, optsList );
    network::Response* response = nullptr;

    do
    {
        network::ParameterList paramList;
        paramList.push_back( std::make_pair( "filtermode", "live" ) );
        paramList.push_back( std::make_pair( "view_id", "1" ) );
        paramList.push_back( std::make_pair( "per_page", "50" ) );
        paramList.push_back( std::make_pair( "page", std::to_string( page++ ) ) );
        request.setParameters( paramList );

        response = mgr->performRequest( &request );
        if ( !response )
        {
            break;
        }
    }
    while ( retrieveOpportunities( response->buffer, opportunities ) > 0 );
}


int main( int argc, char* argv[] )
{
    std::vector<current_rms::opportunity > opportunities;

    network::NetworkManager* mgr = new network::NetworkManager();
    mgr->init();

    // Pull as many opportunities from the server as possible.
    populateFromServer( mgr, &opportunities );
    std::cout << "Collected Opportunities: " << opportunities.size() << std::endl;

    // Calculate the period that were interested in.
    DateTime now = Now();
    printf( "Now: %s\n", tostring(now).c_str() );
    DateTime week_start = addDays( zeroTime( now ), -weekDay( now ) );
    printf( "Start: %s\n", tostring(week_start).c_str() );
    DateTime week_end = addDays( week_start, 7 );
    printf( "End: %s\n", tostring(week_end).c_str() );

    auto fn = [&]( auto& op ){
      return (( op.starts_at >= week_start && op.starts_at < week_end )
          || ( op.ends_at >= week_start && op.ends_at < week_end ));
    };

    // Filter the retrieved opportunities to grab those for the given week.
    std::vector<current_rms::opportunity > filtered =
        filter( fn, opportunities );

    // Lets see how many we've ended up with.
    std::cout << "Filtered Opportunities: " << filtered.size() << std::endl;

    // All of these are from secret file which isn't currently in use?
    oauth2::ClientSpec client_spec{
        googleapis::kClientId,
        googleapis::kClientSecret,
        googleapis::kDefaultAuthUri,
        googleapis::kOutOfBandUrl,
        googleapis::kDefaultRevokeUri,
        googleapis::kDefaultTokenUri
    };

    string scopes = StrCat("email ",
        googleapis::kCalendarScope );

    oauth2::Credential credential;
    googleapi::util::Status status = authenticate( client_spec, scopes, &credential );
    if (!status.ok()) {
      // Fail!!
      return -1;
    }

    mgr->destroy();
    delete mgr;

    return 0;
}
