#pragma once

#include <string>

namespace flagit {
    
    class DataFetcher;

    class FlagIt {
    public:
        // Create instance of this class from 'sourceUrl'.
        explicit FlagIt(std::string sourceUrl) throw(std::invalid_argument);

        // Return true if 'feature' enabled for everyone, false otherwise.
        bool enabled(std::string feature) throw(std::logic_error);

        // Return true if 'feature' enabled for specified 'key', false otherwise.
        bool enabledFor(std::string feature, std::string key) throw(std::logic_error);

        // Return true if 'feature' disabled for specified 'key', false otherwise.
        bool disabledFor(std::string feature, std::string key) throw(std::logic_error);

        // For testing purposes.
        explicit FlagIt(std::shared_ptr<DataFetcher> dataFetcherPtr);
    private:
        // Used by enabled/disabledFor.
        bool contains(std::string const& feature, std::string const& parameter, std::string const& key);

        std::shared_ptr<DataFetcher> m_dataFetcherPtr;
    };

} // close namespace flagit
