#pragma once

#include "AccountModel.h"

void SaveSessionToKeychain (const std::optional<AccountModel::Session> & session);
std::optional<AccountModel::Tokens> LoadSessionFromKeychain ();
AccountModel::Profile ReadProfileFromIdToken (const std::string & id_token);