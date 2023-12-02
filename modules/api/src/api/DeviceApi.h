#pragma once

#include <cpr/cpr.h>
#include <juce_core/juce_core.h>

class DeviceApi
{
public:
    template <class... Ts>
    static cpr::AsyncResponse DeviceCodeRequest (const std::string & base_url)
    {
        auto session = std::make_shared<cpr::Session> ();
        session->SetUrl (cpr::Url {base_url} + cpr::Url {"/device/code"});
        return session->PostAsync ();
    }

    template <class... Ts>
    static cpr::AsyncResponse DeviceTokenRequest (const std::string & base_url,
                                                  const std::string & device_code)
    {
        auto session = std::make_shared<cpr::Session> ();
        session->SetOption (cpr::Url {base_url} + cpr::Url {"/device/token"});
        session->SetOption (
            cpr::Payload {{"grant_type", "urn:ietf:params:oauth:grant-type:device_code"},
                          {"device_code", device_code}});
        return session->PostAsync ();
    }

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
