#ifndef FLAGIT_DATAFETCHER_HH
#define FLAGIT_DATAFETCHER_HH

#include <string>
#include <nlohmann/json.hpp>
#include <thread>

namespace flagit {

    class DataFetcher {
    public:
        // Create instance of this class from 'sourceUrl'. Optionally, specify remoteFetchMs.
        DataFetcher(std::string const& sourceUrl, int remoteFetchMs = 30000);

        // Return data.
        virtual nlohmann::json getData() const noexcept {
            return m_data;
        }

        ~DataFetcher();

    protected:
        // For testing purposes.
        DataFetcher() = default;

    private:
        void refreshFromRemote();

        std::string m_sourceUrl;
        int m_remoteFetchMs;
        nlohmann::json m_data;
        volatile bool m_active;
        std::thread m_remoteRefreshThread;
    };

} // flagit

#endif //FLAGIT_DATAFETCHER_HH
