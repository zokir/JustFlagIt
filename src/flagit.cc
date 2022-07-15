#include "flagit.hh"
#include <curl/curl.h>
#include <iostream>
#include <nlohmann/json.hpp>

namespace {

    size_t writeFunction(void* ptr, size_t size, size_t nmemb, std::string* data) {
        data->append((char*)ptr, size * nmemb);
        return size * nmemb;
    }

nlohmann::json fetchData(std::string const& sourceUrl) {
        std::cout << "Inside of fetchData" << std::endl;
    nlohmann::json result;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL* curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, sourceUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
        curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
        curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_GSSNEGOTIATE);
        curl_easy_setopt(curl,CURLOPT_USERNAME, "");
        curl_easy_setopt(curl,CURLOPT_USERNAME, "");
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "FlagIt");
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        std::string response_string;
        std::string header_string;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_string);

        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        curl = NULL;
        result = nlohmann::json::parse(response_string);
    } else {
        std::cerr << "CURL FAILED" << std::endl;
    }
    return result;
}
} // close namespace<anonymous>

FlagIt::FlagIt(std::string sourceUrl): m_sourceUrl(sourceUrl) {
	// make a call to sourceUrl and parse it out.
    m_root = fetchData(m_sourceUrl);
}

bool FlagIt::enabledFor(std::string feature, std::string key) throw (std::logic_error) {
    std::vector<std::string> keys = value(feature).asList();
    return std::count(keys.begin(), keys.end(), key);
;}

Value FlagIt::value(std::string feature) {
    return Value(m_root[feature.c_str()].get<nlohmann::json>());
}

std::vector<std::string> Value::asList() throw (std::logic_error) {
    if (!m_value.is_array()) {
        // TODO: Print what type is m_value in separate function
        throw std::logic_error("Invalid access, cannot interpret object of type ### as array.");
    }
    return m_value.get<std::vector<std::string>>();
}
