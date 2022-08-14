#ifndef FLAGIT_DATAFETCHER_HH
#define FLAGIT_DATAFETCHER_HH

#include <string>
#include <nlohmann/json.hpp>
#include <thread>

namespace flagit {

    class DataFetcher {
    public:
        // Create instance of this class from 'sourceUrl'. Optionally, specify remoteFetchMs and/or fileFetchMs
        DataFetcher(std::string const& sourceUrl, int remoteFetchMs = 10000, int fileFetchMs = 5000);

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
        void refreshFromFile();

        std::string m_sourceUrl;
        int m_remoteFetchMs;
        int m_fileFetchMs;
        nlohmann::json m_data;
        const std::string m_filePath;
        volatile bool m_active;
        std::thread m_remoteRefreshThread;
        std::thread m_fileRefreshThread;
    };

} // flagit

#endif //FLAGIT_DATAFETCHER_HH
