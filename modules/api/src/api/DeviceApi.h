#pragma once

#include "ApiRequestService.h"
class DeviceApi
{
public:
    struct DeviceCodeSuccessResponse
    {
        juce::String device_code;
        juce::String user_code;
        juce::String verification_uri;
        juce::String verification_uri_complete;
        int expires_in;
        int interval;
    };

    struct DeviceCodeFailResponse
    {
        juce::String error;
    };

    using DeviceCodeCallbacks =
        ApiRequestCallbacks<DeviceCodeSuccessResponse, DeviceCodeFailResponse>;
    static juce::ThreadPoolJob * DeviceCodeRequest (ApiRequestService & api_request_service,
                                                    const DeviceCodeCallbacks & callbacks);

private:
};