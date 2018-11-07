
#include <schedule/api/status.hpp>
#include <schedule/client/status.hpp>

#include <network/uri_utils.hpp>
using network::EscapeForUrl;
#include <foundation/strings/strcat.hpp>
using foundation::StrCat;

#include <string>
#include <iostream>

using std::string;


/*
// Make OAuth flow from client secret path
// Set default scopes [CalendarService::SCOPES::CALENDAR; "https://www.googleapis.com/auth/calendar"] Note that the scopes and service are a seperate "server" vs "client" implementation.
// Set redirect uri (kOutOfBandUrl)
// Set Authorization code callback.  ProptShellForAuthorizationCode.

// Obtains an email from terminal
    // Validate username if entered.  Is just a regex style validation
// RefreshCredentialWithOptions  where the option presented is the email.
    // if refresh token exists do that flow.
    // else
        // authorization_code_callback(actual_options, &auth_code);
        // PerformExchangeAuthorizationCode(auth_code, options, credential);
        //




// this is how we handle not having the authorization code callback?
if (!authorization_code_callback_.get()) {
    const char error[] = "No prompting mechanism provided to get authorization";
    LOG(ERROR) << error;
    return StatusUnimplemented(error);
}

// ------------
// static data!

const char kDefaultAuthUri[] = "https://accounts.google.com/o/oauth2/auth";
const char kDefaultTokenUri[] = "https://accounts.google.com/o/oauth2/token";
const char kDefaultRevokeUri[] = "https://accounts.google.com/o/oauth2/revoke";

const char kOutOfBandUrl[] = "urn:ietf:wg:oauth:2.0:oob";
const char kGoogleAccountsOauth2Url[] = "https://accounts.google.com/o/oauth2";
*/


// ----
// Strip it down.


using googleapi::client::StatusCanceled;
using googleapi::client::StatusInvalidArgument;
using googleapi::client::StatusOk;




const char kDefaultAuthUri[] = "https://accounts.google.com/o/oauth2/auth";
const char kDefaultTokenUri[] = "https://accounts.google.com/o/oauth2/token";
const char kDefaultRevokeUri[] = "https://accounts.google.com/o/oauth2/revoke";
const char kOutOfBandUrl[] = "urn:ietf:wg:oauth:2.0:oob";

static googleapi::util::Status ValidateUserName(const string& name)
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



googleapi::util::Status auth()
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

    string actual_scopes;
    string scopes = "https://www.googleapis.com/auth/calendar";
    if ( !(scopes.find("email ") == 0) && scopes.find(" email") == string::npos) {
        // Add "email" scope if it isnt already present
        actual_scopes = StrCat("email ", scopes);
        scopes = actual_scopes;
    }

    // I'm going to assume these lines are related to glog
    // CHECK(!scopes.empty());
    // CHECK(!client_spec_.client_id().empty()) << "client_id not set";

    // All of these are from the client_spec_  or secret file?
    string client_id;
    string client_secret;
    const string auth_uri = kDefaultAuthUri;
    const string redirect_uri = kOutOfBandUrl;
    const string revoke_uri = kDefaultRevokeUri;
    const string token_uri = kDefaultTokenUri;


    // // credential
    // string access_token_;
    // string refresh_token_;
    // int64 expiration_timestamp_secs_;
    // string email_;
    // bool email_verified_;

    string url =
        StrCat(auth_uri,
                "?client_id=", EscapeForUrl(client_id),
                "&redirect_uri=", EscapeForUrl(redirect_uri),
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
               "&client_id=", EscapeForUrl(client_id),
               "&client_secret=", EscapeForUrl(client_secret),
               "&redirect_uri=", EscapeForUrl(redirect_uri),
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