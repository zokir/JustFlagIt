
#ifndef FLAGIT_MOCKDATAFETCHER_HH
#define FLAGIT_MOCKDATAFETCHER_HH

#include <DataFetcher.hh>

namespace flagit {

    class MockDataFetcher : public DataFetcher {
    public:
        MockDataFetcher(std::string content) : DataFetcher() {
            m_data = nlohmann::json::parse(content);
        }

        // Return data.
        nlohmann::json getData() const noexcept override {
            return m_data;
        }
    private:
        nlohmann::json m_data;

    };

} // close namespace flagit


#endif //FLAGIT_MOCKDATAFETCHER_HH
