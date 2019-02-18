
#include <schedule/opportunity.hpp>
#include <schedule/jsonHelper.hpp>

#define GET_VALUE( name, type ) o.name = json_cast<type>( v[#name] );
#define GET_VALUE_WITH_DEFAULT( name, type, def ) o.name = json_cast_with_default<type >( v[#name], def );

Opportunity convert( rapidjson::Value& v )
{
  Opportunity o;
	GET_VALUE( id, int );
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
	GET_VALUE( created_at, DateTime );
	GET_VALUE( updated_at, DateTime );
  return o;
}
