#include "flagit.hh"
#include <curl/curl.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include "Constants.hh"
#include "DataFetcher.hh"

flagit::FlagIt::FlagIt(std::string sourceUrl) throw(std::invalid_argument)
    : m_dataFetcherPtr(std::make_shared<DataFetcher>(sourceUrl)) {}

bool flagit::FlagIt::enabled(std::string feature) throw (std::logic_error) {
    nlohmann::json data = m_dataFetcherPtr->getData();
    auto const& featureNode = data.find(feature);
    if (featureNode == data.end()) {
        return false;
    }

    auto const& enabledNode = featureNode->find(ENABLED);
    if (enabledNode == featureNode->end()) {
        return false;
    }

    if (!enabledNode->is_boolean()) {
        throw std::logic_error("Enabled value for feature " + feature + " is not a boolean");
    }

    return enabledNode->get<bool>();
}

bool flagit::FlagIt::enabledFor(std::string feature, std::string key) throw (std::logic_error) {
    return contains(feature, ENABLED_FOR, key);
}

bool flagit::FlagIt::disabledFor(std::string feature, std::string key) throw(std::logic_error) {
    return contains(feature, DISABLED_FOR, key);
}

bool flagit::FlagIt::contains(std::string const& feature, std::string const& parameter, std::string const& key) {
    nlohmann::json data = m_dataFetcherPtr->getData();
    auto const& featureNode = data.find(feature);
    if (featureNode == data.end()) {
        return false;
    }

    auto const& parameterNode = featureNode->find(parameter);
    if (parameterNode == featureNode->end()) {
        return false;
    }

    if (!parameterNode->is_array()) {
        throw std::logic_error("DisabledFor value for feature " + feature + " is not a list");
    }

    auto const& keyList = parameterNode->get<std::vector<std::string>>();
    return std::find(keyList.begin(), keyList.end(), key) != keyList.end();
}
