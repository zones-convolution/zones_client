#pragma once

#include <juce_core/juce_core.h>

template <typename S, typename F>
struct ApiRequestCallbacks
{
    using Success = std::function<void (S)>;
    using Fail = std::function<void (F)>;
    using Progress = std::function<void (float)>;
    using Start = std::function<void ()>;

    Success success = nullptr;
    Fail fail = nullptr;
    Progress progress = nullptr;
    Start start = nullptr;
};

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
