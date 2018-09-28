
#include <schedule/api/opportunity.hpp>
#include <schedule/jsonHelper.hpp>

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <iostream>
#include <string>
#include <vector>

namespace current_rms {

struct opportunity
{
	int id;
	int store_id;
	int project_id;
	int member_id;
	int billing_address_id;
	int venue_id;
	int tax_class_id;
	std::string subject;
	std::string description;
	std::string number;
	std::string starts_at;
	std::string ends_at;
	std::string charge_starts_at;
	std::string charge_ends_at;
	std::string ordered_at;
	std::string quote_invalid_at;
	int state;
	std::string state_name;
	int status;
	std::string status_name;
	int use_chargable_days;
	std::string chargable_days;
	bool open_ended_rental;
	bool invoiced;
	int rating;
	std::string revenue;
	bool customer_collecting;
	bool customer_returning;
	std::string reference;
	std::string external_description;
	std::string delivery_instructions;
	int owned_by;
	std::string prep_starts_at;
	std::string prep_end_at;
	std::string load_starts_at;
	std::string load_end_at;
	std::string deliver_starts_at;
	std::string delivery_end_at;
	std::string setup_starts_at;
	std::string setup_end_at;
	std::string show_starts_at;
	std::string show_end_at;
	std::string takedown_starts_at;
	std::string takedown_end_at;
	std::string collect_starts_at;
	std::string collect_end_at;
	std::string unload_starts_at;
	std::string unload_end_at;
	std::string deprep_starts_at;
	std::string deprep_end_at;
	std::string charge_total;
	std::string charge_excluding_total;
	std::string charge_including_total;
	std::string rental_charge_total;
	std::string sales_charge_total;
	std::string surcharge_total;
	std::string service_charge_total;
	std::string tax_total;
	std::string original_rental_charge_total;
	std::string original_sales_charge_total;
	std::string original_service_charge_total;
	std::string original_surcharge_total;
	std::string original_tax_total;
	std::string provisional_cost_total;
	std::string actual_cost_total;
	std::string predicted_cost_total;
	std::string replacement_charge_total;
	std::string weight_total;
	bool item_returned;
	bool prices_include_tax;
	bool pricing_locked;
	//lastest_approval_document_status;
	std::string invoice_charge_total;
	//tag_list
	//assigned_surcharge_group_ids;
	bool has_deal_price;
	bool has_opportunity_deal;
	std::string created_at;
	std::string updated_at;
	//custom_fields
	//owner;
	//member;
	//billing_address;
	//venue;
	//destination;
	//opportunity_surcharges;
	//participants;
};

namespace detail
{
    std::vector<opportunity > opportunities; //< std::pair<uuid, op > ??

	#define GET_VALUE( name, type ) op->name = json_cast<type>( v[#name] );
	#define GET_VALUE_WITH_DEFAULT( name, type, def ) op->name = json_cast_with_default<type >( v[#name], def );

	void convert_internal( rapidjson::Value& v, opportunity* op )
	{
		GET_VALUE( id, int );
		GET_VALUE_WITH_DEFAULT( store_id, int, -1 );
		GET_VALUE_WITH_DEFAULT( project_id, int, -1 );
		GET_VALUE_WITH_DEFAULT( member_id, int, -1 );
		GET_VALUE_WITH_DEFAULT( billing_address_id, int, -1 );
		GET_VALUE_WITH_DEFAULT( venue_id, int, -1 );

		GET_VALUE( subject, const char* );
		GET_VALUE_WITH_DEFAULT( description, const char*, "" );
		GET_VALUE( number, const char* );

		// assert( v["starts_at"].IsString() );
	    // op->starts_at = v["starts_at"].GetString();
		//
		// assert( v["ends_at"].IsString() );
	    // op->ends_at = v["ends_at"].GetString();
		//
		// assert( v["charge_starts_at"].IsString() );
	    // op->charge_starts_at = v["charge_starts_at"].GetString();
		//
		// assert( v["charge_ends_at"].IsString() );
	    // op->charge_ends_at = v["charge_ends_at"].GetString();
		//
		// assert( v["ordered_at"].IsString() );
	    // op->ordered_at = v["ordered_at"].GetString();
		//
		// assert( v["quote_invalid_at"].IsString() );
	    // op->quote_invalid_at = v["quote_invalid_at"].GetString();
		//
		// assert( v["state"].IsInt() );
	    // op->state = v["state"].GetInt();
		//
		// assert( v["state_name"].IsString() );
	    // op->state_name = v["state_name"].GetString();
		//
		// assert( v["status"].IsInt() );
	    // op->status = v["status"].GetInt();
	}
}

}


void current_rms::clearOpportunities()
{
	using namespace current_rms;

	detail::opportunities.clear();
}

std::uint32_t current_rms::getOpportunityCount()
{
	using namespace current_rms;

	return detail::opportunities.size();
}

uuid current_rms::convert( rapidjson::Value& v, current_rms::opportunity** op )
{
	using namespace current_rms;

    detail::opportunities.push_back( opportunity {} );
    opportunity& o = detail::opportunities.back();

    detail::convert_internal( v, &o );

    if ( op != nullptr )
    {
        *op = &o;
    }

    return -1;
}
