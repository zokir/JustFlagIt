#include "DataFetcher.hh"
#include <curl/curl.h>
#include <iostream>
#include <thread>
#include <fstream>
#include <FileUtils.hh>

namespace {

    size_t writeFunction(void* ptr, size_t size, size_t nmemb, std::string* data) {
        data->append((char*)ptr, size * nmemb);
        return size * nmemb;
    }

    // Return json data from 'sourceUrl'. Empty, on failure.
    std::string fetchData(std::string const& sourceUrl) {
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

            std::string response;
            std::string header_string;
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
            curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_string);

            curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            curl = NULL;
            return response;
        } else {
            std::cerr << "CURL FAILED" << std::endl;
        }
        return "";
    }

    // Sleep for 'ms' milliseconds, interrupt if flag is unset.
    void sleepForWithFlag(int ms, volatile bool& flag) {
        int sleepInterval = ms / 100;
        while (ms > 0 && flag) {
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepInterval));
            ms -= sleepInterval;
        }
    }

} // close namespace<anonymous>

namespace flagit {

    DataFetcher::DataFetcher(std::string const &sourceUrl, int remoteFetchMs)
    : m_sourceUrl(sourceUrl), m_remoteFetchMs(remoteFetchMs), m_active(true) {
        // get our config from remote.
        refreshFromRemote();
        // make sure we do this periodically.
        m_remoteRefreshThread = std::thread([this]() {
            while (m_active) {
                refreshFromRemote();
                sleepForWithFlag(m_remoteFetchMs, m_active);
            }
        });
    }

    void DataFetcher::refreshFromRemote() {
        std::string response = fetchData(m_sourceUrl);
        if (response.empty()) {
            // not much we can do, don't mess with existing data.
            return;
        }

        if (!nlohmann::json::accept(response) && m_active) {
            // we only throw if we are still active, no reason to throw in destructor.
            throw std::invalid_argument("Invalid json from remote: " + response);
        }

        m_data = nlohmann::json::parse(response);
    }

    DataFetcher::~DataFetcher() {
        m_active = false;
        if (m_remoteRefreshThread.joinable()) {
            m_remoteRefreshThread.join();
        }
    }
} // flagit