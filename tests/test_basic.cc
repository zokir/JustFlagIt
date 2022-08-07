#include <catch2/catch_test_macros.hpp>

#include <flagit.hh>
#include "MockDataFetcher.hh"

TEST_CASE( "Flag it basic test", "[enabled-feature]" ) {
    std::shared_ptr<flagit::MockDataFetcher> mockDataFetcher = std::make_shared<flagit::MockDataFetcher>(
            "{\"feature1\": {\"enabled\": true, \"enabled_for\": [\"key1\"], \"disabled_for\": [\"key2\"]}}");
    flagit::FlagIt flags(mockDataFetcher);
    REQUIRE(flags.enabled("feature1"));
    REQUIRE_FALSE(flags.enabled("feature3"));
    REQUIRE(flags.enabledFor("feature1", "key1"));
    REQUIRE_FALSE(flags.enabledFor("feature1", "key2"));
    REQUIRE_FALSE(flags.enabledFor("feature1", "key3"));
}

TEST_CASE( "Flag it basic test", "[disabled-feature]" ) {
    std::shared_ptr<flagit::MockDataFetcher> mockDataFetcher = std::make_shared<flagit::MockDataFetcher>(
            "{\"feature2\": {\"enabled\": false, \"enabled_for\": [\"john\", \"alexa\"], \"disabled_for\": [\"alex\"]}}");
    flagit::FlagIt flags(mockDataFetcher);
    REQUIRE_FALSE(flags.enabled("feature2"));
    REQUIRE(flags.enabledFor("feature2", "john"));
    REQUIRE(flags.enabledFor("feature2", "alexa"));
    REQUIRE_FALSE(flags.enabledFor("feature2", "alex"));
    REQUIRE_FALSE(flags.enabledFor("feature2", "robert"));
}
