
#include <schedule/api/status.hpp>
#include <schedule/auth/authenticate.hpp>
#include <schedule/client/status.hpp>

#include <network/uri_utils.hpp>
using network::EscapeForUrl;
#include <foundation/strings/strcat.hpp>
using foundation::StrCat;

#include <string>
using std::string;
#include <iostream>

using googleapi::client::StatusCanceled;
using googleapi::client::StatusInvalidArgument;
using googleapi::client::StatusOk;

googleapi::util::Status ValidateUserName(const string& name)
{
  if (name.find("/") != string::npos)
  {
    return StatusInvalidArgument("UserNames cannot contain '/'");
  }
  else if (name == "." || name == "..")
  {
    return StatusInvalidArgument(
        StrCat("'", name, "' is not a valid UserName"));
  }
  return StatusOk();
}



googleapi::util::Status schedule::authenticate( oauth2::ClientSpec& spec, std::string scopes, oauth2::Credential* credential )
{
    std::cout
        << std::endl
        << "Welcome to the Google APIs for C++ CalendarSample.\n"
        << "  You will need to authorize this program to look at your calendar.\n"
        << "  If you would like to save these credentials between runs\n"
        << "  (or restore from an earlier run) then enter a Google Email "
           "Address.\n"
        << "  Otherwise just press return.\n" << std::endl
        << "  Address: ";
    string email;
    std::getline(std::cin, email);
    if (!email.empty()) {
        googleapi::util::Status status = ValidateUserName(email);
        if (!status.ok()) {
            return status;
        }
    }

    // I'm going to assume these lines are related to glog
    // CHECK(!scopes.empty());
    // CHECK(!client_spec_.client_id().empty()) << "client_id not set";

    string url =
        StrCat(spec.auth_uri_,
               "?client_id=", EscapeForUrl(spec.client_id_),
               "&redirect_uri=", EscapeForUrl(spec.redirect_uri_),
               "&scope=", EscapeForUrl(scopes),
               "&response_type=code");

    std::cout << "Enter the following URL into a browser:\n" << url << std::endl;
    std::cout << std::endl;
    std::cout << "Enter the browser's response to confirm authorization: ";

    string authorization_code;
    std::cin >> authorization_code;
    if ( authorization_code.empty() ) {
        return StatusCanceled("Canceled");
    }

    string content =
        StrCat("code=", EscapeForUrl(authorization_code),
               "&client_id=", EscapeForUrl(spec.client_id_),
               "&client_secret=", EscapeForUrl(spec.client_secret_),
               "&redirect_uri=", EscapeForUrl(spec.redirect_uri_),
               "&grant_type=authorization_code");

    /*
    std::unique_ptr<HttpRequest> request(
        transport_->NewHttpRequest(HttpRequest::POST));
    if (options.timeout_ms > 0) {
        request->mutable_options()->set_timeout_ms(options.timeout_ms);
    }
    request->set_url(client_spec_.token_uri());
    request->set_content_type(HttpRequest::ContentType_FORM_URL_ENCODED);
    request->set_content_reader(NewUnmanagedInMemoryDataReader(content));

    googleapis::util::Status status = request->Execute();
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
}
