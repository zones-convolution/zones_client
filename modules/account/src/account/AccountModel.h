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
        std::string username;
        immer::flex_vector<std::string> groups;
    };

    struct Session
    {
        Tokens tokens;
        Profile profile;
    };

    enum class Status
    {
        kUnauthenticated,
        kLoading,
        kAuthenticated,
    };

    Status status = Status::kUnauthenticated;
    std::optional<Session> session = std::nullopt;
};