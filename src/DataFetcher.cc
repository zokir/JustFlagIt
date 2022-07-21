#include "DataFetcher.hh"
#include <curl/curl.h>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <fstream>

namespace {

    std::string getConfigDir() {
        char const* homeDir;

        if ((homeDir = getenv("HOME")) == nullptr) {
            homeDir = getpwuid(getuid())->pw_dir;
        }

        std::string result = homeDir;
        result += "/.flagit";
        int status = mkdir(result.c_str(), 0755);
        if (status == -1 && errno != EEXIST) {
            throw std::runtime_error("Could not create config directory");
        }

        return result;
    }

    std::string const CONFIG_DIR = getConfigDir();

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

    DataFetcher::DataFetcher(const std::string &sourceUrl, int remoteFetchMs, int fileFetchMs)
    : m_sourceUrl(sourceUrl), m_remoteFetchMs(remoteFetchMs), m_fileFetchMs(fileFetchMs), m_filePath(CONFIG_DIR + "/" + "replace_wirh_url_hash" + ".json") {
        refreshFromRemote();

        std::thread([this]() {
            while (true) {
                std::this_thread::sleep_for(std::chrono::milliseconds(m_fileFetchMs));
                refreshFromFile();
            }
        }).detach();
        std::thread([this]() {
            while (true) {
                std::this_thread::sleep_for(std::chrono::milliseconds(m_remoteFetchMs));
                refreshFromRemote();
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

        // TODO: Get scoped flock on the file, filename.lock etc.
        std::ofstream ofs(m_filePath);
        ofs << response;
        ofs.close();
        // Lets refresh actual data asap.
        refreshFromFile();
    }

    void DataFetcher::refreshFromFile() {
        // TODO: lock, unlock again.
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