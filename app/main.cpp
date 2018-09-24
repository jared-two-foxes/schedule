#include <iostream>
#include <string>
#include <curl/curl.h>


//const std::string authorizeEndPoint = "www.google.com";
//const std::string authorizeEndPoint = "https://twofoxesstyling.current-rms.com/oauth2/authorize";

const std::string endPoint = "https://api.current-rms.com/api/v1";
const std::string subdomain = "twofoxesstyling";
const std::string authToken = "xKC4nNALzixkvovsqmuG";

const std::string opportuntiesEndpoint = "/opportunities?page=1&per_page=20&filtermode=all&view_id=1";


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

    std::cout << readBuffer << std::endl;

    curl_slist_free_all(chunk);
  }

  return 0;
}