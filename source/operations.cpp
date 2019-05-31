
#include <schedule/api/current_rms/current_rms.hpp>
#include <schedule/convert.hpp>
#include <schedule/operations/fetch.hpp>
#include <schedule/operations/transform.hpp>
#include <schedule/opportunity.hpp>
#include <schedule/rest_service.hpp>

#include <foundation/base/functional.hpp>
#include <foundation/logger/logger.hpp>

namespace details {

  std::size_t parseOpportunitiesResponse( std::string const & readBuffer,
    std::vector<Opportunity >* opportunities )
  {
    std::size_t n = 0;

    rapidjson::Document document;
    document.Parse( readBuffer.c_str(), readBuffer.length() );

    assert( document.HasMember("opportunities") );
    assert( document["opportunities"].IsArray() );
    if ( !document["opportunities"].IsNull() )
  	{
      const auto& ar = document["opportunities"].GetArray();
      for ( auto& a : ar )
  		{
        opportunities->push_back( convert( a ) );
        n++;
    	}
    }

    return n;
  }

}

namespace schedule {
namespace operations {

  using namespace details;

  std::vector<Opportunity > fetchFromCurrentRms( std::vector<std::string > const & args )
  {
    // Pull as many opportunities from the server as possible.
    std::vector<Opportunity > opportunities;
    int32_t page = 1;

    while ( true )
    {
      Log( 0, "fetching page '%u'.\n", page );

      std::string uri, content;
      std::vector<std::pair<std::string, std::string > > options;
      current_rms::Opportunities_GET( page++, 50, &uri,
        &options, &content );

      // perform network operation.
      Log( 0, "sending request\n" );
    	std::string payload = RestService::Get( uri, options, content );

      // Parse response
      std::size_t n = parseOpportunitiesResponse( payload, &opportunities );
      Log( 0, "page '%u' contained '%u' opportunities.\n", page, n );

      // break the loop if no opportunities are returned
    	if ( n == 0 )
      {
        break;
      }
    }

    Log( 0, "Filtered Opportunities: %i\n", opportunities.size() );
    return opportunities;
  }

  std::vector<Opportunity > filterByWeek( std::vector<Opportunity > const & opportunities, DateTime const & dateTime )
  {
    // Calculate the period that were interested in.
    DateTime week_start = addDays( zeroTime( dateTime ), -weekDayWithMondayBase( dateTime ) );
    DateTime week_end = addDays( week_start, 7 );

    // Filter the retrieved opportunities to grab those for the given week.
    auto fn = [&]( auto& op ){
      return (( op.starts_at >= week_start && op.starts_at < week_end )
          || ( op.ends_at >= week_start && op.ends_at < week_end ));
    };

    std::vector<Opportunity > out = foundation::filter( fn, opportunities );
    Log( 0, "Filtered Opportunities: %i\n", out.size() );
    return out;
  }

}
}
