#include "OidcApi.h"

OidcApi::DiscoverSuccess OidcApi::ReadDiscoverSuccess (const cpr::Response & response)
{
    auto response_data = juce::JSON::parse (response.text);
    return DiscoverSuccess {
        .authorization_endpoint = response_data.getProperty ("authorization_endpoint", ""),
        .token_endpoint = response_data.getProperty ("token_endpoint", ""),
        .userinfo_endpoint = response_data.getProperty ("userinfo_endpoint", ""),
    };
}

OidcApi::RefreshTokenSuccess OidcApi::ReadRefreshTokenSuccess (const cpr::Response & response)
{
    auto response_data = juce::JSON::parse (response.text);
    return RefreshTokenSuccess {
        .access_token = response_data.getProperty ("access_token", ""),
        .token_type = response_data.getProperty ("token_type", ""),
        .expires_in = response_data.getProperty ("expires_in", 0),
        .scope = juce::StringArray (response_data.getProperty ("scope", juce::Array<juce::var> ())),
        .refresh_token = response_data.getProperty ("refresh_token", ""),
        .id_token = response_data.getProperty ("id_token", ""),
    };
}
