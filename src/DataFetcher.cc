#include "DataFetcher.hh"
#include <curl/curl.h>
#include <iostream>

namespace {

    size_t writeFunction(void* ptr, size_t size, size_t nmemb, std::string* data) {
        data->append((char*)ptr, size * nmemb);
        return size * nmemb;
    }

    nlohmann::json fetchData(std::string const& sourceUrl) {
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
            if (!nlohmann::json::accept(response_string)) {
                throw std::invalid_argument("Invalid json: " + response_string);
            }
            result = nlohmann::json::parse(response_string);
        } else {
            std::cerr << "CURL FAILED" << std::endl;
        }
        return result;
    }
} // close namespace<anonymous>

namespace flagit {
    DataFetcher::DataFetcher(const std::string &sourceUrl, int remoteFetchMs, int fileFetchMs)
    : m_sourceUrl(sourceUrl), m_remoteFetchMs(remoteFetchMs), m_fileFetchMs(fileFetchMs) {
        refreshFromRemote();
        // TODO: Run lambda every 30 seconds in thread
    }

    void DataFetcher::refreshFromRemote() {
        //
        m_data = fetchData(m_sourceUrl);
    }

    void DataFetcher::refreshFromFile() {

    }
} // flagit