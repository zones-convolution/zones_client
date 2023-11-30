#pragma once

#include "AccountModel.h"

#include <lager/effect.hpp>
#include <variant>

struct LoadSessionFromKeychainAction
{
};

struct SaveSessionToKeychainAction
{
};

struct LoadSessionFromTokensAction
{
    AccountModel::Tokens tokens;
};

struct LoadSessionAction
{
    AccountModel::Session session;
};

struct DiscoverOidcAction
{
    std::string base_url;
};

struct DiscoverOidcSuccessAction
{
    OidcApi::DiscoverSuccess discover_success;
};

struct DiscoverOidcFailedAction
{
};

struct RefreshTokenAction
{
};

struct RefreshTokenSuccessAction
{
    AccountModel::Tokens tokens;
};

struct RefreshTokenFailedAction
{
};

using AccountAction = std::variant<LoadSessionFromKeychainAction,
                                   SaveSessionToKeychainAction,
                                   LoadSessionFromTokensAction,
                                   LoadSessionAction,
                                   DiscoverOidcAction,
                                   DiscoverOidcSuccessAction,
                                   DiscoverOidcFailedAction,
                                   RefreshTokenAction,
                                   RefreshTokenSuccessAction,
                                   RefreshTokenFailedAction>;

using AccountResult = lager::result<AccountModel, AccountAction, lager::deps<>>;
AccountResult UpdateAccount (AccountModel model, AccountAction action);
