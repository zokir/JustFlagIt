#ifndef FLAGIT_DATAFETCHER_HH
#define FLAGIT_DATAFETCHER_HH

#include <string>

namespace flagit {
    // Interface for fetching data from remote source.
    class DataFetcher {
    public:
        // Create instance of this class from 'sourceUrl'.
        explicit DataFetcher(std::string sourceUrl = "");

        // Indirection to allow for cleaner override of 'fetchData' from target language.
        std::string getData() {
            return fetchData(m_sourceUrl);
        }

        // Return data in json format. Empty, on failure.
        virtual std::string fetchData(std::string sourceUrl);

        virtual ~DataFetcher() = default;
    private:
        std::string m_sourceUrl;
    };
} // flagit

#endif //FLAGIT_DATAFETCHER_HH
