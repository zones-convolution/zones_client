#include "zones_api/ApiRequest.h"

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

TEST_CASE ("vectors can be sized and resized", "[ApiRequest]")
{
    SECTION ("constructs request with method")
    {
        zones::ApiRequest apiRequest =
            zones::ApiRequest ().withMethod (zones::ApiRequest::HttpVerb::POST, "/newEndpointUrl");

        REQUIRE (apiRequest.httpVerb == zones::ApiRequest::HttpVerb::POST);
    }

    SECTION ("constructs request with field")
    {
        const juce::String field_1 = "f_1";
        const juce::String field_1_value = "f_1_v";
        zones::ApiRequest apiRequest = zones::ApiRequest ().withField (field_1, field_1_value);

        requireFieldCount (apiRequest.fields, 1);
        requireField (apiRequest.fields, field_1, field_1_value);

        const juce::String field_2 = "f_2";
        const juce::String field_2_value = "f_2_v";
        apiRequest = apiRequest.withField (field_2, field_2_value);

        requireFieldCount (apiRequest.fields, 2);
        requireField (apiRequest.fields, field_1, field_1_value);
        requireField (apiRequest.fields, field_2, field_2_value);
    }

    SECTION ("constructs request with header")
    {
        const juce::String header_1 = "h_1";
        const juce::String header_1_value = "h_1_v";
        zones::ApiRequest apiRequest = zones::ApiRequest ().withHeader (header_1, header_1_value);

        REQUIRE (apiRequest.headers.size () == 1);
        REQUIRE (apiRequest.headers.getValue (header_1, "") == header_1_value);

        const juce::String header_2 = "h_2";
        const juce::String header_2_value = "h_2_v";
        apiRequest = apiRequest.withHeader (header_2, header_2_value);

        REQUIRE (apiRequest.headers.size () == 2);
        REQUIRE (apiRequest.headers.getValue (header_2, "") == header_2_value);
    }

    SECTION ("constructs request with base url")
    {
        const juce::String baseUrl = "base_url";
        zones::ApiRequest apiRequest = zones::ApiRequest ().withBaseUrl (baseUrl);

        REQUIRE (apiRequest.baseUrl == baseUrl);
    }
}