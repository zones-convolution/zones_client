#include "ApiRequest.h"

namespace zones
{
static const char * m_httpVerbLiterals [4] = {"GET", "POST", "PUT", "DELETE"};
juce::String ApiRequest::getHttpVerbLiteral (ApiRequest::HttpVerb httpVerb)
{
    return m_httpVerbLiterals [httpVerb];
}

juce::String
ApiRequest::stringPairArrayToHeaderString (const juce::StringPairArray & stringPairArray)
{
    juce::String result;
    for (const auto & key : stringPairArray.getAllKeys ())
    {
        result += key + ": " + stringPairArray.getValue (key, "") + "\n";
    }
    return result;
}

ApiRequest ApiRequest::withMethod (ApiRequest::HttpVerb newHttpVerb,
                                   const juce::String & newEndpointUrl) const
{
    return {.baseUrl = baseUrl,
            .headers = headers,
            .httpVerb = newHttpVerb,
            .endpointUrl = newEndpointUrl,
            .fields = fields};
}

ApiRequest ApiRequest::withField (const juce::String & key, const juce::var & value) const
{
    auto newFields = juce::DynamicObject (fields);
    newFields.setProperty (key, value);

    return {.baseUrl = baseUrl,
            .headers = headers,
            .httpVerb = httpVerb,
            .endpointUrl = endpointUrl,
            .fields = newFields};
}

ApiRequest ApiRequest::withHeader (const juce::String & key, const juce::String & value) const
{
    auto newHeaders = juce::StringPairArray (headers);
    newHeaders.set (key, value);

    return {.baseUrl = baseUrl,
            .headers = newHeaders,
            .httpVerb = httpVerb,
            .endpointUrl = endpointUrl,
            .fields = fields};
}

ApiRequest ApiRequest::withBaseUrl (const juce::String & newBaseUrl)
{
    return {.baseUrl = newBaseUrl,
            .headers = headers,
            .httpVerb = httpVerb,
            .endpointUrl = endpointUrl,
            .fields = fields};
}

juce::URL ApiRequest::getUrlRequest () const
{
    return baseUrl.getChildURL (endpointUrl);
}
}