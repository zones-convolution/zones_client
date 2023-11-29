#pragma once

#include "ApiUtils.h"

#include <cpr/cpr.h>
#include <juce_core/juce_core.h>

class DeviceApi
{
public:
    template <class... Ts>
    static cpr::AsyncResponse DeviceCodeRequest (const std::string & base_url, Ts &&... ts)
    {
        auto session = std::make_shared<cpr::Session> ();
        session->SetUrl (cpr::Url {base_url} + cpr::Url {"/device/code"});
        ApiUtils::ForwardSessionOptions (*session, std::forward<Ts> (ts)...);
        return session->PostAsync ();
    }

    template <class... Ts>
    static cpr::AsyncResponse
    DeviceTokenRequest (const std::string & base_url, const std::string & device_code, Ts &&... ts)
    {
        auto session = std::make_shared<cpr::Session> ();
        session->SetOption (cpr::Url {base_url} + cpr::Url {"/api/device/token"});
        session->SetOption (
            cpr::Payload {{"grant_type", "urn:ietf:params:oauth:grant-type:device_code"},
                          {"device_code", device_code}});
        ApiUtils::ForwardSessionOptions (*session, std::forward<Ts> (ts)...);
        return session->PostAsync ();
    }

    // struct DeviceFailResponse
    // {
    //     juce::String error;
    // };

    // static DeviceFailResponse ReadDeviceFail (const cpr::Response & response);

    struct DeviceCodeSuccess
    {
        juce::String device_code;
        juce::String user_code;
        juce::String verification_uri;
        juce::String verification_uri_complete;
        int expires_in;
        int interval;
    };

    static DeviceCodeSuccess ReadDeviceCodeSuccess (const cpr::Response & response);

    struct DeviceTokenSuccess
    {
        juce::String id_token;
        juce::String access_token;
        juce::String refresh_token;
        int expires_at;
        juce::String token_type;
    };

    static DeviceTokenSuccess ReadDeviceTokenSuccess (const cpr::Response & response);
};
