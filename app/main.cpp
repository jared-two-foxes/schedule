
#include <schedule/api/opportunity.hpp>
#include <schedule/auth/authenticate.hpp>

#include <network/NetworkManager.hpp>
#include <foundation/base/functional.hpp>
using foundation::filter;
#include <rapidjson/document.h>

#include <assert.h>
#include <iostream>
#include <string>
#include <vector>

using namespace schedule;


const std::string API_ENDPOINT = "https://api.current-rms.com/api/v1";
const std::string SUBDOMAIN = "twofoxesstyling";
const std::string OPPORTUNITIES_ENDPOINT = "/opportunities";
const std::string AUTHTOKEN = "xKC4nNALzixkvovsqmuG";




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
    optsList.push_back( std::make_pair( "X-SUBDOMAIN", SUBDOMAIN ) );
    optsList.push_back( std::make_pair( "X-AUTH-TOKEN", AUTHTOKEN ) );

    network::Request request( API_ENDPOINT + OPPORTUNITIES_ENDPOINT, optsList );
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

    authenticate( "client_secret.json" );

    mgr->destroy();
    delete mgr;

    return 0;
}
