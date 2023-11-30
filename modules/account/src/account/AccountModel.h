#pragma once
#include "api/OidcApi.h"
#include "immer/flex_vector.hpp"

#include <optional>
#include <string>

struct AccountModel
{
    struct Tokens
    {
        std::string access_token;
        std::string refresh_token;
        std::string id_token;
    };

    struct Profile
    {
        std::string email;
        std::string username;
        immer::flex_vector<std::string> groups;
    };

    struct Session
    {
        Tokens tokens;
        Profile profile;
    };

    enum class AccountStatus
    {
        kUnauthenticated,
        kLoading,
        kAuthenticated,
    };

    enum class OidcStatus
    {
        kIdle,
        kLoading,
        kSuccess,
        kFailed
    };


    AccountStatus account_status = AccountStatus::kUnauthenticated;
    std::optional<Session> session = std::nullopt;

    OidcStatus oidc_status = OidcStatus::kIdle;
    std::optional<OidcApi::DiscoverSuccess> oidc_discovery = std::nullopt;

    bool is_refreshing_token = false;
};