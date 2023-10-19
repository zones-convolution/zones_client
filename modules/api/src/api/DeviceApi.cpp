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
