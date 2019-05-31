
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
#include <rapidjson/prettywriter.h>

#include <fstream>

using foundation::StrCat;


namespace googleapis {
  const char kClientId[] = "64255872448-1grg1talmvfeui14s4ddh6jhade90l3q.apps.googleusercontent.com";
  const char kClientSecret[] = "rzlhuPrFOwNxXMbC_Ed8AK_L";
  const char kApiKey[]="AIzaSyC2BrDgLTrUKvv4rKHzz_gFlcMTvutIops";
  const char kMapsApiKey[]="AIzaSyD5cje60niMC833wGIJOlu3BsW79TwFVsQ";
  const char kDefaultAuthUri[] = "https://accounts.google.com/o/oauth2/auth";
  const char kDefaultTokenUri[] = "https://accounts.google.com/o/oauth2/token";
  const char kDefaultRevokeUri[] = "https://accounts.google.com/o/oauth2/revoke";
  const char kOutOfBandUrl[] = "urn:ietf:wg:oauth:2.0:oob";
  const char kCalendarScope[] = "https://www.googleapis.com/auth/calendar";
}


// Google Maps API block
int EstimateTravelTime( std::string const & origin, std::string const & destination );

// Google Calendar API Block
void QueryAllCalendars( network::oauth2::Credential const & credential ); 
void QueryCalendarEvents( network::oauth2::Credential const & credential, std::string const & calendarId ); 


// Ini Block.

template <typename T >
void SaveToIni( std::ostream & os, std::string const & key, T const & value ) 
{
  os << key << "=" << value << std::endl;
}

std::string GetNextLineFromIni( std::istream & is )
{
  std::string line;
  is >> line;
  return line;
}

void from_string( std::string const & s, std::string & o )
{
  o = s;
}

void from_string( std::string const & s, int64_t & o ) 
{
  o = std::atol( s.c_str() );
}

template <typename T >
std::string GetPairFromIni( std::string const & line, T & value )
{
  std::vector<std::string > tokens;
  tokens = foundation::split( line, "=" );

  if ( tokens.size() > 1 ) 
  {
    from_string( tokens[1], value );
  }
  else 
  {
    value = {};
  }

  return tokens[0];
}

template <typename T >
void GetFromIni( std::istream& is, std::string & key, T & value )
{
  std::string line = GetNextLineFromIni( is );
  key = GetPairFromIni( line, value );
}


// Credential Block

constexpr const char * const filename = "config.ini";  // Store the config filename global style

network::oauth2::Credential credential;

bool isNull( network::oauth2::Credential const & credential ) 
{
  return credential.access_token_.empty();
}

bool isValid( network::oauth2::Credential const & credential ) 
{
  return ( !isNull(credential) && 
    ( (int64_t)time( nullptr ) < credential.expiration_timestamp_secs_ ) );
}

// Lets store some of this access shit to disk. Gonna store this ini style cause I'm terrible.
void storeCredential( std::string const & name, network::oauth2::Credential const & credential ) 
{
  std::ofstream fs( filename );

  SaveToIni( fs, StrCat("google", ".", "access_token"), 
    credential.access_token_ );
  SaveToIni( fs, StrCat("google", ".", "refresh_token"), 
    credential.refresh_token_ );
  SaveToIni( fs, StrCat("google", ".", "valid_till"), 
    credential.expiration_timestamp_secs_ );
}

network::oauth2::Credential restoreCredential( /*std::string const & name*/ ) 
{
  network::oauth2::Credential c;
  
  std::ifstream fs( filename );
  if (fs.is_open()) 
  {
    std::string key;
    GetFromIni<std::string >( fs, key, c.access_token_ );
    GetFromIni<std::string >( fs, key, c.refresh_token_ );
    GetFromIni<int64_t >( fs, key, c.expiration_timestamp_secs_ );
  }

  return c;
}

network::oauth2::Credential refreshCredential( network::oauth2::Credential const & old )
{
  if ( isValid(old) ) 
  {
    return old;
  }

  network::oauth2::Credential c;
  
  std::vector<std::pair<std::string, std::string > > options;
  options.push_back( std::make_pair("Content-Type", "application/x-www-form-urlencoded") );

  std::string content = StrCat(
    "refresh_token=", old.refresh_token_,
    "&client_id=", googleapis::kClientId,
    "&client_secret=", googleapis::kClientSecret,
    "&grant_type=", "refresh_token" );

  std::string payload = RestService::Post( googleapis::kDefaultTokenUri, options, content );

  //@todo: Handle failure of refresh.

  rapidjson::Document document;
  document.Parse( payload.c_str(), payload.length() );

  // update credential with output from the response
  c.access_token_ = json_cast<const char*>( document, "access_token" );
  c.refresh_token_ = old.refresh_token_;
  int64_t validFor = json_cast<int64_t>( document, "expires_in" );
  c.expiration_timestamp_secs_ = ((int64_t)time( nullptr )) + validFor; 

  // We need to update the config ini file...
  storeCredential( "google", credential );

  credential = c; //< Mutation?
  return c;
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

  return network::oauth2::requestAuth( 
    googleapis::kDefaultAuthUri, client_spec, 
    googleapis::kOutOfBandUrl, scopes );
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

  std::string payload = RestService::Get( googleapis::kDefaultTokenUri, options, content );

  rapidjson::Document document;
  document.Parse( payload.c_str(), payload.length() );

  // update credential with output from the response
  credential.access_token_ = json_cast<const char*>( document, "access_token" );
  credential.refresh_token_ = json_cast<const char*>( document, "refresh_token" );
  int64_t validFor = json_cast<int64_t>( document, "expires_in" );
  credential.expiration_timestamp_secs_ = ((int64_t)time( nullptr )) + validFor; 

  //@todo store only if valid?
  storeCredential( "google", credential );
}


// Google Calendar API Block

void QueryAllCalendars( network::oauth2::Credential const & credential ) 
{
  typedef std::pair<std::string, std::string > StringPair; 
  typedef StringPair OptionPair;

  assert( isValid(credential) );

  std::string content;
  std::vector<OptionPair > options;
  options.push_back( 
    std::make_pair( 
      "Authorization", StrCat( "Bearer ", credential.access_token_ ) ) );
  options.push_back( 
    std::make_pair(
      "Accept", "application/json" ) );

  std::string payload = RestService::Get(
    StrCat(
      "https://www.googleapis.com/calendar/v3/users/me/calendarList",
      "?key=", googleapis::kApiKey ), 
    options, content );

  Log( 10, "\nCalendars\n------\n");

  rapidjson::Document document;
  document.Parse( payload.c_str(), payload.length() );

  // Grab the list of items
  rapidjson::Value& items = document["items"];
  
  std::string calendarId;

  // Grab each calendar item.
  for( rapidjson::Value& i : items.GetArray() )
  {
    const char* title = json_cast<const char* >( i, "summary" );
    bool isPrimary = json_cast_with_default( i, "primary", false );
    Log( 10, "%s%s\n", 
      title, 
      isPrimary ? " (*) " : "" );

    if ( isPrimary )
    {
      calendarId = json_cast<const char* >( i, "id" );
    }
  }

  QueryCalendarEvents( refreshCredential(credential), calendarId );
}


void QueryCalendarEvents( network::oauth2::Credential const & credential, std::string const & calendarId ) 
{
  typedef std::pair<std::string, std::string > StringPair; 
  typedef StringPair OptionPair;

  assert( isValid(credential) );

  Log( 10, "\nEvents\n------\n");

  std::string content;
  std::vector<OptionPair > options;
  options.push_back( 
    std::make_pair( 
      "Authorization", StrCat( "Bearer ", credential.access_token_ ) ) );
  options.push_back( 
    std::make_pair(
      "Accept", "application/json" ) );

  std::string payload = RestService::Get(
    StrCat(
      "https://www.googleapis.com/calendar/v3/calendars/",
      calendarId,
      "/events" ), 
    options, content );

  rapidjson::Document document;
  document.Parse( payload.c_str(), payload.length() );

  // Grab the list of events
  rapidjson::Value& events = document["items"];

  // Grab each event and add to the log.
  for ( rapidjson::Value& v : events.GetArray() )
  {
    Log( 2, "%s\n", json_cast<const char* >(v, "summary") );
  }
}


// Google Maps API block

int EstimateTravelTime( std::string const & origin, std::string const & destination )
{
  typedef std::pair<std::string, std::string > StringPair; 
  typedef StringPair OptionPair;

  const std::string url = "https://maps.googleapis.com/maps/api/directions/";    
  const std::string outputFormat = "json";

  int duration_in_seconds = 0;

  std::string uri = StrCat( url, 
    outputFormat, "?",
    "origin=", origin, 
    "&destination=", destination,
    "&key=", googleapis::kMapsApiKey )  ;

  std::string content;
  std::vector<OptionPair > options;

  std::string payload = RestService::Get(
    uri, options, content );

  //@todo: Handle a failure result?

  // Now lets try to grab the duration from the response.  
  rapidjson::Document document;
  document.Parse( payload.c_str(), payload.length() );

  assert( document.HasMember("routes") );
  rapidjson::Value& routes = document["routes"];
  assert( routes.IsArray() );
  rapidjson::Value& route = routes[0];

  assert( route.HasMember("legs") );
  rapidjson::Value& legs = route["legs"];
  assert( legs.IsArray() );
  for ( rapidjson::Value& leg : legs.GetArray() )
  {
    assert( leg.HasMember("duration") );
    rapidjson::Value& duration = leg["duration"];
    duration_in_seconds += json_cast<int >( duration, "value" );;
  }

  return ( duration_in_seconds / 60 );
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

  // lets attempt to load from file.
  credential = restoreCredential( /*"google"*/ );
}

void Application::run()
{
  Log( 0, "Starting main loop.\n" );

  std::string line;
  std::vector<std::string > args;

  while (!quit_)
  {
    if ( isNull(credential) ) 
    {
      renderer_.addLine( "Please navigate to the bellow address and follow the onscreen prompts to allow access to the google servers" );
      std::string address = announceAutheticationRequest();
      renderer_.addLine( address );
    }

    if ( !displayList_.empty() ) 
    {
      renderer_.addOpportunities( displayList_ );
    }

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
    DateTime date;
    if ( args.size() >= 2 ) {
      date = Parse( args[1], "DD/MM" );
    } 
    else {
      date = Now();
    }
    displayList_ = filterByWeek( masterOpportunitiesList_, date ); // args
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
  
  // Google Sheets functions
  // auto push_fn = [this](const std::vector<std::string >& args) {
  //   pushToGoogleSheet();
  // };

  // Google Calendar functions
  auto query_fn = [this](std::vector<std::string > const & args ) {
    QueryAllCalendars( refreshCredential( credential ) );
  };

  // Google Maps functions
  auto estimate_fn = [this](std::vector<std::string > const & args ) {
    //auto op = displayList_[0];
    int time = EstimateTravelTime( "44+Henderson+Valley+Road,Henderson,Auckland", "2+Bellamont+street,Huapai" );
    int breakpoint = 12;
  };

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
        {"query", query_fn},
        {"estimate", estimate_fn},
        //{"push", push_fn}
      }};
}

// void Application::pushToGoogleSheet()
// {
//   //TODO: check for google authorization.
//   //TODO: send data to sheet.
// }

