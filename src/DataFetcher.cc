#include "DataFetcher.hh"
#include <curl/curl.h>
#include <thread>
#include <iostream>

namespace  {
    size_t writeFunction(void* ptr, size_t size, size_t nmemb, std::string* data) {
        data->append((char*)ptr, size * nmemb);
        return size * nmemb;
    }
}

flagit::DataFetcher::DataFetcher(std::string sourceUrl) : m_sourceUrl(std::move(sourceUrl)) {}

std::string flagit::DataFetcher::fetchData(std::string sourceUrl) {
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
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1L);
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        std::string response;
        std::string header_string;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_string);

        int maxAttempt = 3; // completely arbitrary
        int maxSleepBetweenRetryMs = 50; // completely arbitrary
        while (maxAttempt-- > 0 && curl_easy_perform(curl) != CURLE_OK) {
            response.clear();
            std::this_thread::sleep_for(std::chrono::milliseconds(maxSleepBetweenRetryMs));
        }

        curl_easy_cleanup(curl);
        curl = NULL;
        return response;
    }
    return "";
}