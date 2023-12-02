#pragma once

#include "AccountModel.h"

#include <lager/effect.hpp>
#include <variant>

struct ClearSessionAction
{
};

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

struct DeviceCodeAction
{
};

struct DeviceCodeSuccessAction
{
    DeviceApi::DeviceCodeSuccess device_code_success;
};

struct DeviceCodeFailedAction
{
};

struct PollDeviceTokenAction
{
    std::string device_code;
};

using AccountAction = std::variant<ClearSessionAction,
                                   LoadSessionFromKeychainAction,
                                   SaveSessionToKeychainAction,
                                   LoadSessionFromTokensAction,
                                   LoadSessionAction,
                                   DiscoverOidcAction,
                                   DiscoverOidcSuccessAction,
                                   DiscoverOidcFailedAction,
                                   RefreshTokenAction,
                                   RefreshTokenSuccessAction,
                                   RefreshTokenFailedAction,
                                   DeviceCodeAction,
                                   DeviceCodeSuccessAction,
                                   DeviceCodeFailedAction,
                                   PollDeviceTokenAction>;

using AccountResult = lager::result<AccountModel, AccountAction, lager::deps<>>;
AccountResult UpdateAccount (AccountModel model, AccountAction action);
