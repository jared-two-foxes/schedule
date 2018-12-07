
#include <schedule/api/opportunity.hpp>
#include <schedule/jsonHelper.hpp>

#include <network/network.hpp>
#include <network/status.hpp>
#include <network/auth/authenticate.hpp>
#include <network/transport/response.hpp>
#include <network/transport/router.hpp>
#include <network/transport/router_locator.hpp>

#include <foundation/base/functional.hpp>
#include <foundation/strings/strcat.hpp>

#include <rapidjson/document.h>

#include <assert.h>
#include <iostream>
#include <string>
#include <vector>

using namespace network;
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

void populateFromServer( Router* router,
        std::vector<current_rms::opportunity >* opportunities )
{
    int32_t page = 1;
    network::util::Status status;

    // Set up transport layer options.
    network::ParameterList optsList;
    optsList.push_back(std::make_pair("X-SUBDOMAIN", currentrms::kSubdomain ));
    optsList.push_back(std::make_pair("X-AUTH-TOKEN", currentrms::kAuthToken ));

    network::Request request;
    request.uri_ = currentrms::kOpportunitiesUri;
    request.options_ = optsList;

    network::Response response;

    do
    {
        request.parameters_.push_back( std::make_pair( "filtermode", "live" ) );
        request.parameters_.push_back( std::make_pair( "view_id", "1" ) );
        request.parameters_.push_back( std::make_pair( "per_page", "50" ) );
        request.parameters_.push_back( std::make_pair( "page", std::to_string( page++ ) ) );

        response.buffer_.clear();
        status = router->perform( request, &response );
        if ( !status.ok() )
        {
            break;
        }
    }
    while ( retrieveOpportunities( response.buffer_, opportunities ) > 0 );
}

network::util::Status authenticateGoogleapis( network::Router* router )
{
    network::Request request;
    network::Response response;
    network::util::Status status;

    request.options_.push_back( std::make_pair("Content-Type", "application/x-www-form-urlencoded"));

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

    status = network::oauth2::requestAuth( client_spec, scopes, &request );
    if ( !status.ok() ) {
        std::cout << status.ToString() << std::endl;
        return status;
    }

    std::cout << "Enter the following URL into a browser:\n" << request.uri_ << std::endl;
    std::cout << std::endl;
    std::cout << "Enter the browser's response to confirm authorization: ";

    string authorization_code;
    std::cin >> authorization_code;
    if ( authorization_code.empty() ) {
        std::cout << status.ToString() << std::endl;
        return network::util::StatusCanceled("Canceled");
    }

    status = network::oauth2::confirmAuth( client_spec, authorization_code, &request );
    if ( !status.ok() ) {
        std::cout << status.ToString() << std::endl;
        return status;
    }

    status = router->perform( request, &response );

    //@todo: Check the response?
    oauth2::Credential credential;
    if (status.ok()) {
        // update credential with output from the response

        rapidjson::Document document;
        document.Parse( response.buffer_.c_str(), response.buffer_.length() );

        credential.access_token_ = json_cast<const char*>( document["access_token"] );
        credential.refresh_token_ = json_cast<const char*>( document["refresh_token"] );
        // credential.expiration_timestamp_secs_; /*int64_t*/
    }



    /*
    std::unique_ptr<HttpRequest> request(
        transport_->NewHttpRequest(HttpRequest::POST));
    if (options.timeout_ms > 0) {
        request->mutable_options()->set_timeout_ms(options.timeout_ms);
    }
    request->set_url(client_spec_.token_uri());
    request->set_content_type(HttpRequest::ContentType_FORM_URL_ENCODED);
    request->set_content_reader(NewUnmanagedInMemoryDataReader(content));

    networks::util::Status status = request->Execute();
    if (status.ok()) {
    if (status.ok() && check_email_ && !options.email.empty()) {
        status = credential->Update(request->response()->body_reader());
            if (options.email != credential->email()) {
                status = StatusUnknown(
                    StrCat("Credential email address mismatch. Expected [",
                        options.email, "] but got [", credential->email(), "]"));
                credential->Clear();
            }
        }
    }*/

    return status;
}


int main( int argc, char* argv[] )
{
    std::vector<current_rms::opportunity > opportunities;

    network::init();
    network::Router* router = network::RouterLocator::getRouter();

    // Pull as many opportunities from the server as possible.
    populateFromServer( router, &opportunities );
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

    authenticateGoogleapis(router);

    network::destroy();

    return 0;
}
