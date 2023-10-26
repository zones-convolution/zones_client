#pragma once
#include "ApiRequestService.h"

class OidcApi
{
public:
    struct DiscoverFailResponse
    {
    };

    struct DiscoverSuccessResponse
    {
        juce::String authorization_endpoint;
        juce::String token_endpoint;
        juce::String userinfo_endpoint;
    };

    using DiscoverCallbacks = ApiRequestCallbacks<DiscoverSuccessResponse, DiscoverFailResponse>;
    static juce::ThreadPoolJob * DiscoverRequest (ApiRequestService & api_request_service,
                                                  const juce::String & oidc_provider,
                                                  const DiscoverCallbacks & callbacks);

    struct TokenFailResponse
    {
    };

    struct TokenSuccessResponse
    {
    };

    using TokenCallbacks = ApiRequestCallbacks<TokenSuccessResponse, TokenFailResponse>;
    static juce::ThreadPoolJob * TokenRequest (ApiRequestService & api_request_service,
                                               const juce::String & token_endpoint,
                                               const DiscoverCallbacks & callbacks);
};
