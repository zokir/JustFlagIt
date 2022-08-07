#ifndef FLAGIT_DATAFETCHER_HH
#define FLAGIT_DATAFETCHER_HH

#include <string>
#include <nlohmann/json.hpp>

namespace flagit {

    class DataFetcher {
    public:
        // Create instance of this class from 'sourceUrl'. Optionally, specify remoteFetchMs and/or fileFetchMs
        DataFetcher(std::string const& sourceUrl, int remoteFetchMs = 10000, int fileFetchMs = 5000);

        // Return data.
        virtual nlohmann::json getData() const noexcept {
            return m_data;
        }

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
        std::string m_filePath;
    };

} // flagit

#endif //FLAGIT_DATAFETCHER_HH
