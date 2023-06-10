#pragma once

#include <juce_core/juce_core.h>

namespace zones
{
struct ApiRequest
{
public:
    enum HttpVerb
    {
        GET,
        POST,
        PUT,
        DELETE
    };

    static juce::String
    stringPairArrayToHeaderString (const juce::StringPairArray & stringPairArray);
    static juce::String getHttpVerbLiteral (ApiRequest::HttpVerb httpVerb);

    [[nodiscard]] ApiRequest withBaseUrl (const juce::String & newBaseUrl);
    [[nodiscard]] ApiRequest withMethod (HttpVerb newHttpVerb,
                                         const juce::String & newEndpointUrl) const;
    [[nodiscard]] ApiRequest withField (const juce::String & key, const juce::var & value) const;
    [[nodiscard]] ApiRequest withHeader (const juce::String & key,
                                         const juce::String & value) const;
    [[nodiscard]] juce::URL getUrlRequest () const;

    juce::URL baseUrl;
    juce::StringPairArray headers;
    HttpVerb httpVerb;
    juce::String endpointUrl;
    juce::DynamicObject fields;
};
}
