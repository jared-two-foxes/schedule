
#include <schedule/application.hpp>
#include <schedule/api/current_rms/current_rms.hpp>
#include <schedule/jsonHelper.hpp>
#include <schedule/operations/fetch.hpp>
#include <schedule/operations/transform.hpp>
#include <schedule/opportunity.hpp>
#include <schedule/rest_service.hpp>

#include <foundation/logger/logger.hpp>
#include <foundation/strings/strcat.hpp>
#include <network/network.hpp>
#include <network/auth/authenticate.hpp>
#include <network/transport/router_locator.hpp>

#include <rapidjson/document.h>


using foundation::StrCat;


namespace googleapis {
  const char kClientId[] = "64255872448-1grg1talmvfeui14s4ddh6jhade90l3q.apps.googleusercontent.com";
  const char kClientSecret[] = "rzlhuPrFOwNxXMbC_Ed8AK_L";
  const char kDefaultAuthUri[] = "https://accounts.google.com/o/oauth2/auth";
  const char kDefaultTokenUri[] = "https://accounts.google.com/o/oauth2/token";
  const char kDefaultRevokeUri[] = "https://accounts.google.com/o/oauth2/revoke";
  const char kOutOfBandUrl[] = "urn:ietf:wg:oauth:2.0:oob";
  const char kCalendarScope[] = "https://www.googleapis.com/auth/calendar";
}


std::string payload;
network::oauth2::Credential credential;

bool isValid( network::oauth2::Credential const & credential ) {
  return !credential.access_token_.empty();
}

std::string announceAutheticationRequest()
{
  // All of these are from secret file which isn't currently in use?
  network::oauth2::ClientInfo client_spec{
    googleapis::kClientId,
    googleapis::kClientSecret
  };

  std::string scopes = StrCat("email ",
    googleapis::kCalendarScope );

  payload = network::oauth2::requestAuth( googleapis::kDefaultAuthUri, client_spec, googleapis::kOutOfBandUrl, scopes );

  return payload;
}

void obtainAuthorizationToken( std::string const & authorization_code )
{
  network::oauth2::ClientInfo client_spec{
    googleapis::kClientId,
    googleapis::kClientSecret
  };

  std::vector<std::pair<std::string, std::string > > options;
  options.push_back( std::make_pair("Content-Type", "application/x-www-form-urlencoded") );

  std::string content = network::oauth2::confirmAuth( client_spec, googleapis::kOutOfBandUrl, authorization_code );

  payload = RestService::Send( RestService::GET, googleapis::kDefaultTokenUri, options, content );

  rapidjson::Document document;
  document.Parse( payload.c_str(), payload.length() );

  // update credential with output from the response
  credential.access_token_ = json_cast<const char*>( document["access_token"] );
  credential.refresh_token_ = json_cast<const char*>( document["refresh_token"] );
  // credential.expiration_timestamp_secs_; /*int64_t, document["expires_in"]*/
}

Application::Application() :
  quit_(false)
{}

Application::~Application()
{
  network::destroy();
}

void Application::setup()
{
  network::init();
  RestService::SetTransportLayer( network::RouterLocator::getRouter() );
  initDispatcher();

  announceAutheticationRequest();
}

void Application::run()
{
  Log( 0, "Starting main loop.\n" );

  std::string line;
  std::vector<std::string > args;

  while (!quit_)
  {
    if ( !isValid(credential) ) {
      renderer_.addLine( payload );
      renderer_.addLine( " " );
    }
    renderer_.addOpportunities( displayList_ );

    terminal_ = renderer_.present( terminal_ );

    // Grab the next operation from the command line.
    getline( std::cin, line );

    // Pre-process command, split commands and arguments, and send to the
    // command processor.
    terminal_ = terminal_.append( line + '\n' );
    args = foundation::split( line, " " );

    processCmd( args[0], args );
  }
}

void Application::processCmd( std::string const& cmd, std::vector<std::string > const& args )
{
  dispatcher_.process( cmd, args );
}

void Application::initDispatcher()
{
  using namespace schedule::operations;

  // Lifecycle functions.
  auto quit_fn = [this](const std::vector<std::string >& args) {
    Log( 0, "Quiting..." );
    quit_ = true;
  };

  // current_rms Functions.
  auto fetch_fn  = [this](const std::vector<std::string >& args ) {
    masterOpportunitiesList_ = fetchFromCurrentRms( args );
    displayList_ = masterOpportunitiesList_;
  };
  auto filter_fn = [this](const std::vector<std::string > & args) {
    displayList_ = filterByWeek( masterOpportunitiesList_, Now() ); // args
  };
  auto next_fn = [this](const std::vector<std::string > & args) {
    renderer_.advanceElements();
  };
  auto prev_fn = [this](const std::vector<std::string > & args) {
    int32_t num = -1;
    if ( args.size() > 1 )
    {
      num = atoi(args[1].c_str());
    }
    renderer_.advanceElements(num);
  };

  // Authentication functions
  auto authorize_fn = [this](const std::vector<std::string>& args) {
    assert( args.size() > 1 );
    obtainAuthorizationToken( args[1] );
  };
  
  // google Sheets functions
  // auto push_fn = [this](const std::vector<std::string >& args) {
  //   pushToGoogleSheet();
  // };

  // Create the actual dispatcher
  dispatcher_ =
    framework::CommandDispatcher<
      std::string, void (std::vector<std::string > const& ) > {{
        {"quit", quit_fn},
        {"fetch", fetch_fn},
        {"next", next_fn},
        {"prev", prev_fn},
        {"filter", filter_fn},
        {"auth", authorize_fn},
        //{"push", push_fn}
      }};
}

// void Application::pushToGoogleSheet()
// {
//   //TODO: check for google authorization.
//   //TODO: send data to sheet.
// }
