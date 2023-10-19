#include "DeviceApi.h"

#include <juce_core/juce_core.h>

juce::ThreadPoolJob *
DeviceApi::DeviceCodeRequest (ApiRequestService & api_request_service,
                              const DeviceApi::DeviceCodeCallbacks & callbacks)
{
    auto device_code_request = ApiRequest ()
                                   .WithBaseUrl ("http://localhost:3000")
                                   .WithMethod (ApiRequest::HttpVerb::kPost, "/api/device/code");
    return api_request_service.BeginRequest (
        device_code_request,
        {
            .success =
                [=] (auto result)
            {
                auto response_data = juce::JSON::parse (result.data.toString ());
                callbacks.success ({
                    .device_code = response_data.getProperty ("device_code", ""),
                    .user_code = response_data.getProperty ("user_code", ""),
                    .verification_uri = response_data.getProperty ("verification_uri", ""),
                    .verification_uri_complete =
                        response_data.getProperty ("verification_uri_complete", ""),
                    .expires_in = response_data.getProperty ("expires_in", 0),
                    .interval = response_data.getProperty ("interval", 0),
                });
            },
            .fail =
                [=] (auto result)
            {
                auto response_data = juce::JSON::parse (result.data.toString ());
                callbacks.fail ({.error = response_data.getProperty ("error", "")});
            },
            .progress = [=] (auto progress) { callbacks.progress (progress); },
            .start = [=] { callbacks.start (); },
        });
}

juce::ThreadPoolJob *
DeviceApi::DeviceTokenRequest (ApiRequestService & api_request_service,
                               const juce::String & device_code,
                               const DeviceApi::DeviceTokenCallbacks & callbacks)
{
    auto device_token_request =
        ApiRequest ()
            .WithBaseUrl ("http://localhost:3000")
            .WithMethod (ApiRequest::HttpVerb::kPost, "/api/device/token")
            .WithField ("grant_type", "urn:ietf:params:oauth:grant-type:device_code")
            .WithField ("device_code", device_code);

    return api_request_service.BeginRequest (
        device_token_request,
        {
            .success =
                [=] (auto result)
            {
                auto response_data = juce::JSON::parse (result.data.toString ());
                callbacks.success ({
                    .id_token = response_data.getProperty ("id_token", ""),
                    .access_token = response_data.getProperty ("access_token", ""),
                    .refresh_token = response_data.getProperty ("refresh_token", ""),
                    .expires_at = response_data.getProperty ("device_code", 0),
                    .token_type = response_data.getProperty ("token_type", ""),
                });
            },
            .fail =
                [=] (auto result)
            {
                auto response_data = juce::JSON::parse (result.data.toString ());
                callbacks.fail ({.error = response_data.getProperty ("error", "")});
            },
            .progress = [=] (auto progress) { callbacks.progress (progress); },
            .start = [=] { callbacks.start (); },
        });
}
