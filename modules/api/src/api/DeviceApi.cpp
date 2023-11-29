#include "DeviceApi.h"

DeviceApi::DeviceCodeSuccess DeviceApi::ReadDeviceCodeSuccess (const cpr::Response & response)
{
    auto response_data = juce::JSON::parse (response.text);
    return DeviceCodeSuccess {
        .device_code = response_data.getProperty ("device_code", ""),
        .user_code = response_data.getProperty ("user_code", ""),
        .verification_uri = response_data.getProperty ("verification_uri", ""),
        .verification_uri_complete = response_data.getProperty ("verification_uri_complete", ""),
        .expires_in = response_data.getProperty ("expires_in", 0),
        .interval = response_data.getProperty ("interval", 0),
    };
}

DeviceApi::DeviceTokenSuccess DeviceApi::ReadDeviceTokenSuccess (const cpr::Response & response)
{
    auto response_data = juce::JSON::parse (response.text);
    return DeviceTokenSuccess {
        .id_token = response_data.getProperty ("id_token", ""),
        .access_token = response_data.getProperty ("access_token", ""),
        .refresh_token = response_data.getProperty ("refresh_token", ""),
        .expires_at = response_data.getProperty ("device_code", 0),
        .token_type = response_data.getProperty ("token_type", ""),
    };
}
