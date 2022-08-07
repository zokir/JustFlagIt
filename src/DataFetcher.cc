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

} // close namespace<anonymous>

namespace flagit {

    DataFetcher::DataFetcher(std::string const &sourceUrl, int remoteFetchMs, int fileFetchMs)
    : m_sourceUrl(sourceUrl), m_remoteFetchMs(remoteFetchMs), m_fileFetchMs(fileFetchMs),
    m_filePath(details::getFilePath(sourceUrl)) {
        // get our config from remote.
        refreshFromRemote();
        // make sure we do this periodically.
        std::thread([this]() {
            while (true) {
                std::this_thread::sleep_for(std::chrono::milliseconds(m_remoteFetchMs));
                refreshFromRemote();
            }
        }).detach();
        // refresh more frequently from file. This would help if we have more processes using this url.
        std::thread([this]() {
            while (true) {
                std::this_thread::sleep_for(std::chrono::milliseconds(m_fileFetchMs));
                refreshFromFile();
            }
        }).detach();
    }

    void DataFetcher::refreshFromRemote() {
        std::string response = fetchData(m_sourceUrl);
        if (response.empty()) {
            // not much we can do, don't mess with existing data.
            return;
        }

        if (!nlohmann::json::accept(response)) {
            throw std::invalid_argument("Invalid json: " + response);
        }

        // TODO: Need to make a call whether we want to lock the file
        // or whether its safer to leave it alone and just deal with
        // the fact that sometimes we would fail to refresh the data.
        std::ofstream ofs(m_filePath);
        ofs << response;
        ofs.close();
        // Lets refresh actual data asap.
        refreshFromFile();
    }

    void DataFetcher::refreshFromFile() {
        // TODO: Same lock dilemma.
        std::ifstream t(m_filePath);
        std::stringstream buffer;
        buffer << t.rdbuf();
        std::string content = buffer.str();
        if (!nlohmann::json::accept(content)) {
            throw std::invalid_argument("Invalid json: " + content);
        }
        m_data = nlohmann::json::parse(content);
    }
} // flagit