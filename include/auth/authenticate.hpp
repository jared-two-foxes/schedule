#ifndef SCHEDULE_AUTHENTICATE_HPP_
#define SCHEDULE_AUTHENTICATE_HPP_

#include <schedule/api/status.hpp>

#include <string>
using std::string;

namespace schedule {

namespace oauth2 {

/*
 * Specifies an OAuth 2.0 Credential.
 */
struct Credential
{
  string access_token_;
  string refresh_token_;
  int64_t expiration_timestamp_secs_;
  // string email_;
  // bool email_verified_;
};

/*
 * A data object containing specifying the client information to present to
 * the OAuth 2.0 server.
 */
struct ClientSpec
{
  string client_id_;
  string client_secret_;
  string auth_uri_;
  string redirect_uri_;
  string revoke_uri_; // ??
  string token_uri_;
};

}

googleapi::util::Status authenticate( /*transportlayer* transport,*/
  oauth2::ClientSpec& spec, std::string scopes, oauth2::Credential* credential );

}

#endif // SCHEDULE_AUTHENTICATE_HPP_
