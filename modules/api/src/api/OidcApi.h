#pragma once

#include <cpr/cpr.h>
#include <juce_core/juce_core.h>

class OidcApi
{
public:
    template <class... Ts>
    static cpr::AsyncResponse DiscoverRequest (const std::string & base_url, Ts &&... ts)
    {
        auto session = std::make_shared<cpr::Session> ();
        session->SetUrl (cpr::Url {base_url} + cpr::Url {"/.well-known/openid-configuration"});
        return session->GetAsync ();
    }

    template <class... Ts>
    static cpr::AsyncResponse RefreshTokenRequest (const std::string & token_endpoint,
                                                   const std::string & refresh_token,
                                                   Ts &&... ts)
    {
        auto session = std::make_shared<cpr::Session> ();
        session->SetUrl (cpr::Url {token_endpoint});
        session->SetPayload (
            cpr::Payload {{"grant_type", "refresh_token"}, {"scope", "offline_access openid"}});
        return session->PostAsync ();
    }

    struct DiscoverSuccess
    {
        juce::String authorization_endpoint;
        juce::String token_endpoint;
        juce::String userinfo_endpoint;
    };

    static DiscoverSuccess ReadDiscoverSuccess (const cpr::Response & response);

    struct RefreshTokenSuccess
    {
        juce::String access_token;
        juce::String token_type;
        int expires_in;
        juce::StringArray scope;
        juce::String refresh_token;
        juce::String id_token;
    };

    static RefreshTokenSuccess ReadRefreshTokenSuccess (const cpr::Response & response);
};
