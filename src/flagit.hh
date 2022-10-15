#ifndef FLAGIT_HH
#define FLAGIT_HH

#include <string>
#include <stdexcept>
#include <memory>
#include "DataFetcher.hh"
#include <iostream>

namespace flagit {
    
    class DataRefresher;

    class FlagIt {
    public:
        // Create instance of this class from 'sourceUrl'. Optionally, specify remoteFetchMs.
        FlagIt(std::string sourceUrl, int remoteFetchMs = 30000);

        // Create instance of this class from 'dataFetcher'. Optionally, specify remoteFetchMs.
        FlagIt(flagit::DataFetcher* dataFetcher, int remoteFetchMs = 30000);

        // Return true if 'feature' enabled for everyone, false otherwise.
        bool enabled(std::string feature);

        // Return true if 'feature' enabled for specified 'key', false otherwise.
        bool enabledFor(std::string feature, std::string key);

        // Return true if 'feature' disabled for specified 'key', false otherwise.
        bool disabledFor(std::string feature, std::string key);

        // For testing purposes.
        explicit FlagIt(std::shared_ptr<DataRefresher> dataFetcherPtr);
    private:
        // Used by enabled/disabledFor.
        bool contains(std::string const& feature, std::string const& parameter, std::string const& key);

        std::shared_ptr<DataRefresher> m_dataRefresherPtr;
    };

} // close namespace flagit

#endif //FLAGIT_HH