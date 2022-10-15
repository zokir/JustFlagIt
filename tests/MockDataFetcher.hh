
#ifndef FLAGIT_MOCKDATAFETCHER_HH
#define FLAGIT_MOCKDATAFETCHER_HH

#include <DataRefresher.hh>

namespace flagit {

    class MockDataFetcher : public DataRefresher {
    public:
        MockDataFetcher(std::string content) : DataRefresher() {
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
