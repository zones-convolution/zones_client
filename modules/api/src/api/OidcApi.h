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

    struct RefreshTokenFailResponse
    {
    };

    struct RefreshTokenSuccessResponse
    {
        juce::String access_token;
        juce::String token_type;
        int expires_in;
        juce::StringArray scope;
        juce::String refresh_token;
        juce::String id_token;
    };

    using RefreshTokenCallbacks =
        ApiRequestCallbacks<RefreshTokenSuccessResponse, RefreshTokenFailResponse>;
    static juce::ThreadPoolJob * RefreshTokenRequest (ApiRequestService & api_request_service,
                                                      const juce::String & token_endpoint,
                                                      const juce::String & refresh_token,
                                                      const RefreshTokenCallbacks & callbacks);
};
