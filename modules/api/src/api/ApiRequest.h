#pragma once

#include <juce_core/juce_core.h>

struct ApiRequest
{
public:
    enum class HttpVerb
    {
        kGet,
        kPost,
        kPut,
        kDelete
    };

    static juce::String
    StringPairArrayToHeaderString (const juce::StringPairArray & string_pair_array);
    static juce::String GetHttpVerbLiteral (ApiRequest::HttpVerb http_verb);

    [[nodiscard]] ApiRequest WithBaseUrl (const juce::String & new_base_url);
    [[nodiscard]] ApiRequest WithMethod (HttpVerb new_http_verb,
                                         const juce::String & new_endpoint_url) const;
    [[nodiscard]] ApiRequest WithField (const juce::String & key, const juce::var & value) const;
    [[nodiscard]] ApiRequest WithHeader (const juce::String & key,
                                         const juce::String & value) const;
    [[nodiscard]] juce::URL GetUrlRequest () const;

    juce::URL base_url;
    juce::StringPairArray headers;
    HttpVerb http_verb;
    juce::String endpoint_url;
    juce::DynamicObject fields;
};
