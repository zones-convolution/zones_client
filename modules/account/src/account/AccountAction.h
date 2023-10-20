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

using AccountAction = std::variant<LoadSessionFromKeychainAction, SaveSessionToKeychainAction>;
using AccountEffect = lager::effect<AccountAction, lager::deps<>>;
using AccountResult = std::pair<AccountModel, AccountEffect>;

AccountResult UpdateAccount (AccountModel model, AccountAction action);
