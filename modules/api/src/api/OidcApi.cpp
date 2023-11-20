#include "OidcApi.h"

juce::ThreadPoolJob * OidcApi::DiscoverRequest (ApiRequestService & api_request_service,
                                                const juce::String & oidc_provider,
                                                const OidcApi::DiscoverCallbacks & callbacks)
{
    auto discover_request =
        ApiRequest ()
            .WithBaseUrl (oidc_provider)
            .WithMethod (ApiRequest::HttpVerb::kGet, "/.well-known/openid-configuration");

    return api_request_service.BeginRequest (
        discover_request,
        {
            .success =
                [=] (auto result)
            {
                auto response_data = juce::JSON::parse (result.data.toString ());
                callbacks.success ({
                    .authorization_endpoint =
                        response_data.getProperty ("authorization_endpoint", ""),
                    .token_endpoint = response_data.getProperty ("token_endpoint", ""),
                    .userinfo_endpoint = response_data.getProperty ("userinfo_endpoint", ""),
                });
            },
            .fail = [=] (auto result) { callbacks.fail ({}); },
            .progress = [=] (auto progress) { callbacks.progress (progress); },
            .start = [=] { callbacks.start (); },
        });
}

juce::ThreadPoolJob *
OidcApi::RefreshTokenRequest (ApiRequestService & api_request_service,
                              const juce::String & token_endpoint,
                              const juce::String & refresh_token,
                              const OidcApi::RefreshTokenCallbacks & callbacks)
{
    auto refresh_token_request =
        ApiRequest ()
            .WithMethod (ApiRequest::HttpVerb::kPost, token_endpoint)
            .WithField ("grant_type", "refresh_token")
            .WithField ("scope", juce::StringArray ({"offline_access", "openid"}))
            .WithField ("refresh_token", refresh_token);

    return api_request_service.BeginRequest (
        refresh_token_request,
        {
            .success =
                [=] (auto result)
            {
                juce::var response_data = juce::JSON::parse (result.data.toString ());
                callbacks.success ({
                    .access_token = response_data.getProperty ("access_token", ""),
                    .token_type = response_data.getProperty ("token_type", ""),
                    .expires_in = response_data.getProperty ("expires_in", 0),
                    .scope = juce::StringArray (
                        response_data.getProperty ("scope", juce::Array<juce::var> ())),
                    .refresh_token = response_data.getProperty ("refresh_token", ""),
                    .id_token = response_data.getProperty ("id_token", ""),
                });
            },
            .fail = [=] (auto result) { callbacks.fail ({}); },
            .progress = [=] (auto progress) { callbacks.progress (progress); },
            .start = [=] { callbacks.start (); },
        });
}
