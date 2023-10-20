#pragma once
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
        std::string name;
        std::string image_url;
        immer::flex_vector<std::string> roles;
    };

    struct Session
    {
        Tokens tokens;
        Profile profile;
    };

    enum class Status
    {
        kLoading,
        kAuthenticated,
        kUnauthenticated
    };

    Status status;
    std::optional<Session> session;
};