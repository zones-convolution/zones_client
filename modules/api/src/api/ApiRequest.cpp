#include "ApiRequest.h"

juce::String ApiRequest::GetHttpVerbLiteral (ApiRequest::HttpVerb http_verb)
{
    switch (http_verb)
    {
        case HttpVerb::kGet:
            return "GET";
        case HttpVerb::kPost:
            return "POST";
        case HttpVerb::kPut:
            return "PUT";
        case HttpVerb::kDelete:
            return "DELETE";
    }
}

juce::String
ApiRequest::StringPairArrayToHeaderString (const juce::StringPairArray & string_pair_array)
{
    juce::String result;
    for (const auto & key : string_pair_array.getAllKeys ())
    {
        result += key + ": " + string_pair_array.getValue (key, "") + "\n";
    }
    return result;
}

ApiRequest ApiRequest::WithMethod (HttpVerb new_http_verb,
                                   const juce::String & new_endpoint_url) const
{
    return {.base_url = base_url,
            .headers = headers,
            .http_verb = new_http_verb,
            .endpoint_url = new_endpoint_url,
            .fields = fields};
}

ApiRequest ApiRequest::WithField (const juce::String & key, const juce::var & value) const
{
    auto newFields = juce::DynamicObject (fields);
    newFields.setProperty (key, value);

    return {.base_url = base_url,
            .headers = headers,
            .http_verb = http_verb,
            .endpoint_url = endpoint_url,
            .fields = newFields};
}

ApiRequest ApiRequest::WithHeader (const juce::String & key, const juce::String & value) const
{
    auto newHeaders = juce::StringPairArray (headers);
    newHeaders.set (key, value);

    return {.base_url = base_url,
            .headers = newHeaders,
            .http_verb = http_verb,
            .endpoint_url = endpoint_url,
            .fields = fields};
}

ApiRequest ApiRequest::WithBaseUrl (const juce::String & new_base_url)
{
    return {.base_url = new_base_url,
            .headers = headers,
            .http_verb = http_verb,
            .endpoint_url = endpoint_url,
            .fields = fields};
}

juce::URL ApiRequest::GetUrlRequest () const
{
    return base_url.getChildURL (endpoint_url);
}