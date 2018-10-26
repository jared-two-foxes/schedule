#ifndef CURRENTRMS_API_OPPORTUNITY_HPP__
#define CURRENTRMS_API_OPPORTUNITY_HPP__

#include <schedule/DateTime.hpp>
#include <foundation/uuid.hpp>
#include <rapidjson/document.h>


namespace current_rms {

    struct opportunity
    {
    	int id;
    	int store_id;
    	int project_id;
    	int member_id;
        // int billing_address_id;
    	int venue_id;
    	// int tax_class_id;
    	std::string subject;
    	std::string description;
    	std::string number;
    	DateTime starts_at;
    	DateTime ends_at;
    	// DateTime charge_starts_at;
    	// DateTime charge_ends_at;
    	// DateTime ordered_at;
    	// DateTime quote_invalid_at;
    	// int state;
    	std::string state_name;
    	// int status;
    	std::string status_name;
    	//int use_chargable_days;
    	//std::string chargable_days;
    	//bool open_ended_rental;
    	//bool invoiced;
    	//int rating;
    	//std::string revenue;
    	bool customer_collecting;
    	bool customer_returning;
    	std::string reference;
    	//std::string external_description;
    	std::string delivery_instructions;
    	//int owned_by;
    	// DateTime prep_starts_at;
    	// DateTime prep_end_at;
    	// DateTime load_starts_at;
    	// DateTime load_end_at;
    	DateTime deliver_starts_at;
    	DateTime deliver_ends_at;
    	// DateTime setup_starts_at;
    	// DateTime setup_end_at;
    	// DateTime show_starts_at;
    	// DateTime show_end_at;
    	// DateTime takedown_starts_at;
    	// DateTime takedown_end_at;
    	DateTime collect_starts_at;
    	DateTime collect_ends_at;
    	// DateTime unload_starts_at;
    	// DateTime unload_end_at;
    	// DateTime deprep_starts_at;
    	// DateTime deprep_end_at;
    	// std::string charge_total;
    	// std::string charge_excluding_total;
    	// std::string charge_including_total;
    	// std::string rental_charge_total;
    	// std::string sales_charge_total;
    	// std::string surcharge_total;
    	// std::string service_charge_total;
    	// std::string tax_total;
    	// std::string original_rental_charge_total;
    	// std::string original_sales_charge_total;
    	// std::string original_service_charge_total;
    	// std::string original_surcharge_total;
    	// std::string original_tax_total;
    	// std::string provisional_cost_total;
    	// std::string actual_cost_total;
    	// std::string predicted_cost_total;
    	// std::string replacement_charge_total;
    	// std::string weight_total;
        bool item_returned;
    	// bool prices_include_tax;
    	// bool pricing_locked;
    	//lastest_approval_document_status;
    	//std::string invoice_charge_total;
    	//tag_list
    	//assigned_surcharge_group_ids;
    	//bool has_deal_price;
    	//bool has_opportunity_deal;
    	DateTime created_at;
    	DateTime updated_at;
    	//custom_fields
    	//owner;
    	//member;
    	//billing_address;
    	//venue;
    	//destination;
    	//opportunity_surcharges;
    	//participants;
    };

    opportunity convert( rapidjson::Value& v );

}

#endif //
