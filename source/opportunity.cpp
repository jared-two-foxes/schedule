
#include <schedule/api/opportunity.hpp>
#include <schedule/jsonHelper.hpp>

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <iostream>
#include <string>
#include <vector>

namespace current_rms {
namespace detail {

#define GET_VALUE( name, type ) op->name = json_cast<type>( v[#name] );
#define GET_VALUE_WITH_DEFAULT( name, type, def ) op->name = json_cast_with_default<type >( v[#name], def );

void convert_internal( rapidjson::Value& v, opportunity* op )
{
	GET_VALUE( id, int );
	GET_VALUE_WITH_DEFAULT( store_id, int, -1 );
	GET_VALUE_WITH_DEFAULT( project_id, int, -1 );
	GET_VALUE_WITH_DEFAULT( member_id, int, -1 );
	GET_VALUE_WITH_DEFAULT( venue_id, int, -1 );
	GET_VALUE( subject, const char* );
	GET_VALUE_WITH_DEFAULT( description, const char*, "" );
	GET_VALUE( number, const char* );
	GET_VALUE( starts_at, DateTime );
	GET_VALUE( ends_at, DateTime );
	GET_VALUE( state_name, const char* );
	GET_VALUE( status_name, const char* );
	GET_VALUE( customer_collecting, bool );
	GET_VALUE( customer_returning, bool );
	GET_VALUE( delivery_instructions, const char* );
	GET_VALUE_WITH_DEFAULT( deliver_starts_at, DateTime, DateTime() );
	GET_VALUE_WITH_DEFAULT( deliver_ends_at, DateTime, DateTime() );
	GET_VALUE_WITH_DEFAULT( collect_starts_at, DateTime, DateTime() );
	GET_VALUE_WITH_DEFAULT( collect_ends_at, DateTime, DateTime() );
	GET_VALUE( item_returned, bool );
	GET_VALUE( created_at, DateTime );
	GET_VALUE( updated_at, DateTime );
}

}
}

current_rms::opportunity current_rms::convert( rapidjson::Value& v )
{
		using namespace current_rms;
	  opportunity o = opportunity {};
	  detail::convert_internal( v, &o );
	  return o;
}
