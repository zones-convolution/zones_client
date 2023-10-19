#include "api/ApiRequest.h"

#include <catch2/catch_test_macros.hpp>
#include <juce_core/juce_core.h>

static void requireField (juce::DynamicObject & fields,
                          const juce::Identifier & identifier,
                          const juce::var & value)
{
    REQUIRE (fields.getProperty (identifier) == value);
}

static void requireFieldCount (juce::DynamicObject & fields, int count)
{
    REQUIRE (fields.getProperties ().size () == count);
}

TEST_CASE ("api request tests", "[ApiRequest]")
{
    SECTION ("constructs request with method")
    {
        auto api_request =
            ApiRequest ().WithMethod (ApiRequest::HttpVerb::kPost, "/newEndpointUrl");
        REQUIRE (api_request.http_verb == ApiRequest::HttpVerb::kPost);
    }

    SECTION ("constructs request with field")
    {
        const juce::String field_1 = "f_1";
        const juce::String field_1_value = "f_1_v";
        auto api_request = ApiRequest ().WithField (field_1, field_1_value);

        requireFieldCount (api_request.fields, 1);
        requireField (api_request.fields, field_1, field_1_value);

        const juce::String field_2 = "f_2";
        const juce::String field_2_value = "f_2_v";
        api_request = api_request.WithField (field_2, field_2_value);

        requireFieldCount (api_request.fields, 2);
        requireField (api_request.fields, field_1, field_1_value);
        requireField (api_request.fields, field_2, field_2_value);
    }

    SECTION ("constructs request with header")
    {
        const juce::String header_1 = "h_1";
        const juce::String header_1_value = "h_1_v";
        auto api_request = ApiRequest ().WithHeader (header_1, header_1_value);

        REQUIRE (api_request.headers.size () == 1);
        REQUIRE (api_request.headers.getValue (header_1, "") == header_1_value);

        const juce::String header_2 = "h_2";
        const juce::String header_2_value = "h_2_v";
        api_request = api_request.WithHeader (header_2, header_2_value);

        REQUIRE (api_request.headers.size () == 2);
        REQUIRE (api_request.headers.getValue (header_2, "") == header_2_value);
    }

    SECTION ("constructs request with base url")
    {
        const juce::String kBaseUrl = "base_url";
        ApiRequest api_request = ApiRequest ().WithBaseUrl (kBaseUrl);
        REQUIRE (api_request.base_url == kBaseUrl);
    }
}