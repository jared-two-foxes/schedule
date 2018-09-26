
#include <iostream>
#include <string>
#include <vector>

#include <curl/curl.h>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

//const std::string authorizeEndPoint = "www.google.com";
//const std::string authorizeEndPoint = "https://twofoxesstyling.current-rms.com/oauth2/authorize";

const std::string endPoint = "https://api.current-rms.com/api/v1";
const std::string subdomain = "twofoxesstyling";
const std::string authToken = "xKC4nNALzixkvovsqmuG";

const std::string opportuntiesEndpoint = "/opportunities?page=1&per_page=20&filtermode=live&view_id=1";


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

std::vector<opportunity > opportunities;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main(int argc, char* argv[])
{
  CURL *curl;
  CURLcode res;
  std::string readBuffer;

  curl = curl_easy_init();
  if(curl) {
 	std::string subdomainheaderopt = std::string("X-SUBDOMAIN: ") + subdomain;
 	std::string authheaderopt = std::string("X-AUTH-TOKEN: ") + authToken;

	struct curl_slist *chunk = NULL;
    chunk = curl_slist_append(chunk, subdomainheaderopt.c_str() );
    chunk = curl_slist_append(chunk, authheaderopt.c_str() );

	res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

	std::string request = endPoint + opportuntiesEndpoint;

    curl_easy_setopt(curl, CURLOPT_URL, request.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

	//std::cout << readBuffer << std::endl;

	//@todo: create a structure to represent the opportunity data from server.
	//@todo: iterate all the "pages"
	//@todo: store opportunities that fall within a given date period.

	rapidjson::Document document;
	document.Parse(readBuffer);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	document.Accept(writer);
	std::cout << buffer.GetString() << std::endl;

    curl_slist_free_all(chunk);
  }

  return 0;
}
