#pragma once

#include "ApiRequestService.h"
class DeviceApi
{
public:
    struct DeviceFailResponse
    {
        juce::String error;
    };

    struct DeviceCodeSuccessResponse
    {
        juce::String device_code;
        juce::String user_code;
        juce::String verification_uri;
        juce::String verification_uri_complete;
        int expires_in;
        int interval;
    };

    using DeviceCodeCallbacks = ApiRequestCallbacks<DeviceCodeSuccessResponse, DeviceFailResponse>;
    static juce::ThreadPoolJob * DeviceCodeRequest (ApiRequestService & api_request_service,
                                                    const DeviceCodeCallbacks & callbacks);

    struct DeviceTokenSuccessResponse
    {
        juce::String id_token;
        juce::String access_token;
        juce::String refresh_token;
        int expires_at;
        juce::String token_type;
    };

    using DeviceTokenCallbacks =
        ApiRequestCallbacks<DeviceTokenSuccessResponse, DeviceFailResponse>;
    static juce::ThreadPoolJob * DeviceTokenRequest (ApiRequestService & api_request_service,
                                                     const juce::String & device_code,
                                                     const DeviceTokenCallbacks & callbacks);
};
