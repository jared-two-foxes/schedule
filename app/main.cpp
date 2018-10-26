
#include <schedule/api/opportunity.hpp>
#include <schedule/functional.hpp>

#include <network/network.hpp>
#include <rapidjson/document.h>

#include <assert.h>
#include <iostream>
#include <string>
#include <vector>


const std::string API_ENDPOINT = "https://api.current-rms.com/api/v1";
const std::string SUBDOMAIN = "twofoxesstyling";
const std::string OPPORTUNITIES_ENDPOINT = "/opportunities";
const std::string AUTHTOKEN = "xKC4nNALzixkvovsqmuG";



static size_t WriteCallback( void *contents, size_t size, size_t nmemb, void *userp )
{
    ((std::string*)userp)->append( (char*)contents, size * nmemb );
    return size * nmemb;
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

void populateFromServer( std::string endpoint,
        std::vector<current_rms::opportunity >* opportunities )
{
    std::string readBuffer;
    int32_t page = 1;

    // Set up header
    network::ParameterList optsList;
    optsList.push_back( std::make_pair( "X-SUBDOMAIN", SUBDOMAIN ) );
    optsList.push_back( std::make_pair( "X-AUTH-TOKEN", AUTHTOKEN ) );
    network::setupHttpHeader( optsList );

    do
    {
        readBuffer.clear();

        network::ParameterList paramList;
        paramList.push_back( std::make_pair( "filtermode", "live" ) );
        paramList.push_back( std::make_pair( "view_id", "1" ) );
        paramList.push_back( std::make_pair( "per_page", "50" ) );
        paramList.push_back( std::make_pair( "page", std::to_string( page++ ) ) );
        network::setUrlAndParameters( API_ENDPOINT + endpoint, paramList );

        network::setCallback( WriteCallback, &readBuffer );

        if ( !network::performOperation() )
        {
            break;
        }
    }
    while ( retrieveOpportunities( readBuffer, opportunities ) > 0 );
}


int main( int argc, char* argv[] )
{
    std::vector<current_rms::opportunity > opportunities;

    // Pull as many opportunities from the server as possible.
    network::init();
    populateFromServer( OPPORTUNITIES_ENDPOINT, &opportunities );
    std::cout << "Collected Opportunities: " << opportunities.size() << std::endl;

    // Calculate the period that were interested in.
    DateTime now = Now();
    printf( "Now: %s\n", tostring(now).c_str() );
    DateTime week_start = addDays( zeroTime( now ), -weekDay( now ) );
    printf( "Start: %s\n", tostring(week_start).c_str() );
    DateTime week_end = addDays( week_start, 7 );
    printf( "End: %s\n", tostring(week_end).c_str() );

    // Filter the retrieved opportunities to grab those for the given week.
    std::vector<current_rms::opportunity > filtered =
        filter( opportunities, [&]( auto& op ){
            return (( op.starts_at >= week_start && op.starts_at < week_end )
                || ( op.ends_at >= week_start && op.ends_at < week_end ));
            });

    // Lets see how many we've ended up with.
    std::cout << "Filtered Opportunities: " << filtered.size() << std::endl;

    network::quit();
    return 0;
}
