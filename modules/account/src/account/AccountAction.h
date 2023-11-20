#pragma once

#include "AccountModel.h"

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
using AccountResult = lager::result<AccountModel, AccountAction, lager::deps<>>;
AccountResult UpdateAccount (AccountModel model, AccountAction action);
