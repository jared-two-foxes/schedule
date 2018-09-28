
#include <schedule/api/opportunity.hpp>

#include <rapidjson/document.h>

#include <curl/curl.h>

#include <assert.h>
#include <iostream>
#include <string>
#include <vector>

const std::string API_ENDPOINT = "https://api.current-rms.com/api/v1";
const std::string SUBDOMAIN = "twofoxesstyling";
const std::string OPPORTUNITIES_ENDPOINT = "/opportunities";

const std::string AUTHTOKEN = "xKC4nNALzixkvovsqmuG";


std::string readBuffer;
std::vector<current_rms::opportunity* > opportunities;


struct Options {
    //filterMode;
    std::uint32_t view_id;
    std::uint32_t count;
};


static size_t WriteCallback( void *contents, size_t size, size_t nmemb, void *userp )
{
    ((std::string*)userp)->append( (char*)contents, size * nmemb );
    return size * nmemb;
}

struct curl_slist* setRequestHeader( CURL* curl )
{
    CURLcode res;
    std::string subdomainheaderopt = std::string("X-SUBDOMAIN: ") + SUBDOMAIN;
 	std::string authheaderopt = std::string("X-AUTH-TOKEN: ") + AUTHTOKEN;
    struct curl_slist *chunk = NULL;

    chunk = curl_slist_append( chunk, subdomainheaderopt.c_str() );
    chunk = curl_slist_append( chunk, authheaderopt.c_str() );

	res = curl_easy_setopt( curl, CURLOPT_HTTPHEADER, chunk );

    return chunk;
}

CURLcode setUrlAndParameters( CURL* curl, std::string endpoint, Options& options, std::uint32_t page )
{
    std::string request = API_ENDPOINT + endpoint;
    request += "?filtermode=live";
    request += "&view_id=" + std::to_string( options.view_id );
    request += "&page=" + std::to_string( page );
    request += "&per_page=" + std::to_string( options.count );

    curl_easy_setopt( curl, CURLOPT_URL, request.c_str() );
    curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, WriteCallback );
    curl_easy_setopt( curl, CURLOPT_WRITEDATA, &readBuffer );
}

CURLcode performOperation( CURL* curl )
{
    CURLcode res;
    res = curl_easy_perform( curl );
    return res;
}

void populateFromServer( CURL* curl, std::string endpoint )
{
    CURLcode res;

    //set up header
    struct curl_slist* chunk = setRequestHeader( curl );

    Options options;
    options.view_id = 1;
    options.count = 50;

    std::uint32_t page = 1;
    while (1)
    {
        readBuffer.clear();

        res = setUrlAndParameters( curl, endpoint, options, page++ );
        if ( res != CURLE_OK )
        {
            break;
        }

        res = performOperation( curl );
        if ( res != CURLE_OK )
        {
            break;
        }

        rapidjson::Document document;
    	document.Parse( readBuffer.c_str(), readBuffer.length() );

    	assert( document.HasMember("opportunities") );
    	assert( document["opportunities"].IsArray() );
    	if ( !document["opportunities"].IsNull() )
    	{
            const auto& ar = document["opportunities"].GetArray();
            if ( ar.Size() == 0 )
            {
                break;
            }

    		for ( auto& a : ar )
    		{
    			//opportunity* op = nullptr;
    			uuid id = current_rms::convert( a, nullptr );
    			//opportunities.push_back( op );
    		}
    	}

        std::cout << "Opportunities: " << current_rms::getOpportunityCount() << std::endl;
    }

    curl_slist_free_all(chunk);
}




//@todo: create a structure to represent the opportunity data from server.
//@todo: iterate all the "pages"
//@todo: store opportunities that fall within a given date period.


int main( int argc, char* argv[] )
{
    CURL* curl = curl_easy_init();
    if( curl )
    {
        // Pull as many opportunities from the server as possible.
        populateFromServer( curl, OPPORTUNITIES_ENDPOINT );

        // Lets see how many we got.
        std::cout << "Collected Opportunities: " << current_rms::getOpportunityCount() << std::endl;

        // Clean up and lets leave!
        curl_easy_cleanup( curl );
    }

    return 0;
}


// rapidjson::StringBuffer buffer;
// rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
// document.Accept(writer);
//std::cout << buffer.GetString() << std::endl;
