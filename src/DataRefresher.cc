#include "DataRefresher.hh"
#include <iostream>
#include <thread>
#include <fstream>

namespace {

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

    DataRefresher::DataRefresher(std::unique_ptr<DataFetcher> dataFetcher, int remoteFetchMs)
    : m_dataFetcherPtr(std::move(dataFetcher)), m_remoteFetchMs(remoteFetchMs), m_active(true) {
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

    void DataRefresher::refreshFromRemote() {
        std::string response = m_dataFetcherPtr->getData();
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

    DataRefresher::~DataRefresher() {
        m_active = false;
        if (m_remoteRefreshThread.joinable()) {
            m_remoteRefreshThread.join();
        }
    }
} // flagit
