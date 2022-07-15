#pragma once

#include <string>
#include <nlohmann/json.hpp>

struct Value {
    Value(nlohmann::json const& value): m_value(value) {}
    std::vector<std::string> asList() throw (std::logic_error);
    nlohmann::json m_value;
};

class FlagIt {
public:
    FlagIt(std::string sourceUrl);
    Value value(std::string feature);
    bool enabledFor(std::string feature, std::string key) throw (std::logic_error);
private:
    std::string m_sourceUrl;
    nlohmann::json m_root;
};
