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
