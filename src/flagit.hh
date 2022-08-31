#pragma once

#include <string>
#include <stdexcept>
#include <memory>

namespace flagit {
    
    class DataFetcher;

    class FlagIt {
    public:
        // Create instance of this class from 'sourceUrl'.
        explicit FlagIt(std::string sourceUrl);

        // Return true if 'feature' enabled for everyone, false otherwise.
        bool enabled(std::string feature);

        // Return true if 'feature' enabled for specified 'key', false otherwise.
        bool enabledFor(std::string feature, std::string key);

        // Return true if 'feature' disabled for specified 'key', false otherwise.
        bool disabledFor(std::string feature, std::string key);

        // For testing purposes.
        explicit FlagIt(std::shared_ptr<DataFetcher> dataFetcherPtr);
    private:
        // Used by enabled/disabledFor.
        bool contains(std::string const& feature, std::string const& parameter, std::string const& key);

        std::shared_ptr<DataFetcher> m_dataFetcherPtr;
    };

} // close namespace flagit
