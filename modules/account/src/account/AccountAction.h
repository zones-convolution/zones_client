#pragma once

#include "AccountModel.h"
#include "api/ApiRequestService.h"

#include <filesystem>
#include <lager/effect.hpp>
#include <lager/util.hpp>
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

using AccountAction = std::variant<LoadSessionFromKeychainAction,
                                   SaveSessionToKeychainAction,
                                   LoadSessionFromTokensAction,
                                   LoadSessionAction>;
using AccountDeps = lager::deps<ApiRequestService &>;
using AccountResult = lager::result<AccountModel, AccountAction, AccountDeps>;
AccountResult UpdateAccount (AccountModel model, AccountAction action);
