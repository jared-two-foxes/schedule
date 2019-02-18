


#include <schedule/application.hpp>


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



namespace googleapis {
  const char kClientId[] = "64255872448-1grg1talmvfeui14s4ddh6jhade90l3q.apps.googleusercontent.com";
  const char kClientSecret[] = "rzlhuPrFOwNxXMbC_Ed8AK_L";
  const char kDefaultAuthUri[] = "https://accounts.google.com/o/oauth2/auth";
  const char kDefaultTokenUri[] = "https://accounts.google.com/o/oauth2/token";
  const char kDefaultRevokeUri[] = "https://accounts.google.com/o/oauth2/revoke";
  const char kOutOfBandUrl[] = "urn:ietf:wg:oauth:2.0:oob";

  const char kCalendarScope[] = "https://www.googleapis.com/auth/calendar";
}

network::util::Status authenticateGoogleapis( network::Router* router )
{
    network::Request request;
    network::Response response;
    network::util::Status status;

    request.options_.push_back( std::make_pair("Content-Type", "application/x-www-form-urlencoded"));

    // All of these are from secret file which isn't currently in use?
    oauth2::ClientSpec client_spec {
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
