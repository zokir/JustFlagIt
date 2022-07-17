#ifndef FLAGIT_DATAFETCHER_HH
#define FLAGIT_DATAFETCHER_HH

#include <string>
#include <nlohmann/json.hpp>

namespace flagit {

    class DataFetcher {
    public:
        // Create instance of this class from 'sourceUrl'. Optionally, specify remoteFetchMs and/or fileFetchMs
        DataFetcher(std::string const& sourceUrl, int remoteFetchMs = 5000, int fileFetchMs = 1000);

        // Return data.
        nlohmann::json getData() const noexcept {
            return m_data;
        }
    private:
        void refreshFromRemote();
        void refreshFromFile();

        std::string m_sourceUrl;
        int m_remoteFetchMs;
        int m_fileFetchMs;
        nlohmann::json m_data;
    };

} // flagit

#endif //FLAGIT_DATAFETCHER_HH
