#ifndef FLAGIT_DATAREFRESHER_HH
#define FLAGIT_DATAREFRESHER_HH

#include <string>
#include <nlohmann/json.hpp>
#include <thread>
#include <DataFetcher.hh>

namespace flagit {

    class DataRefresher {
    public:
        // Create instance of this class from 'sourceUrl'. Optionally, specify remoteFetchMs.
        DataRefresher(std::unique_ptr<DataFetcher> dataFetcher, int remoteFetchMs = 30000);

        // Return data.
        virtual nlohmann::json getData() const noexcept {
            return m_data;
        }

        ~DataRefresher();

    protected:
        // For testing purposes.
        DataRefresher() = default;

    private:
        void refreshFromRemote();

        std::unique_ptr<DataFetcher> m_dataFetcherPtr;
        int m_remoteFetchMs;
        nlohmann::json m_data;
        volatile bool m_active;
        std::thread m_remoteRefreshThread;
    };

} // flagit

#endif //FLAGIT_DATAREFRESHER_HH
