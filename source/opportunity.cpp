
#include <schedule/api/opportunity.hpp>

#include <string>
#include <vector>

struct opportunity {
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

namespace api {
namespace detail {
    std::vector<opportunity > opportunities; //< std::pair<uuid, op > ??
}
}

void convert_internal( rapidjson::Value& v, opportunity* op )
{
    op->id = v["id"].GetInt();
    op->store_id = v["store_id"].GetInt();
    op->project_id = v["project_id"].GetInt();
    op->member_id = v["member_id"].GetInt();
    op->billing_address_id = v["billing_address_id"].GetInt();
    op->venue_id = v["venue_id"].GetInt();
    op->subject = v["subject"].GetString();
    op->description = v["description"].GetString();
    op->number = v["number"].GetString();
    op->starts_at = v["starts_at"].GetString();
    op->ends_at = v["ends_at"].GetString();
    op->charge_starts_at = v["charge_starts_at"].GetString();
    op->charge_ends_at = v["charge_ends_at"].GetString();
    op->ordered_at = v["ordered_at"].GetString();
    op->quote_invalid_at = v["quote_invalid_at"].GetString();
    op->state = v["state"].GetInt();
    op->state_name = v["state_name"].GetString();
    op->status = v["status"].GetInt();
}

uuid convert( rapidjson::Value& v, opportunity** op )
{
    using namespace api::detail;

    opportunities.push_back( opportunity {} );
    opportunity& o = opportunities.back();

    convert_internal( v, &o );

    if ( op != nullptr )
    {
        *op = &o;
    }

    return -1;
}
