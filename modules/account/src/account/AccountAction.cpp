#include "AccountAction.h"

#include <keychain/keychain.h>

const std::string kPackage = "com.zones";
const std::string kUser = "Admin";

const std::string kAccessTokenKey = "access_token";
const std::string kRefreshTokenKey = "refresh_token";
const std::string kIdTokenKey = "id_token";

static void SaveSessionToKeychain (const std::optional<AccountModel::Session> & session)
{
    keychain::Error error {};

    if (session.has_value ())
    {
        keychain::setPassword (
            kPackage, kAccessTokenKey, kUser, session->tokens.access_token, error);
        keychain::setPassword (
            kPackage, kRefreshTokenKey, kUser, session->tokens.refresh_token, error);
        keychain::setPassword (kPackage, kIdTokenKey, kUser, session->tokens.id_token, error);
    }
    else
    {
        keychain::deletePassword (kPackage, kAccessTokenKey, kUser, error);
        keychain::deletePassword (kPackage, kRefreshTokenKey, kUser, error);
        keychain::deletePassword (kPackage, kIdTokenKey, kUser, error);
    }
}

AccountResult UpdateAccount (AccountModel model, AccountAction action)
{
    return std::visit (
        lager::visitor {
            [&] (const LoadSessionFromKeychainAction & _) -> AccountResult
            {
                return {model, [] (auto && context) {

                        }};
            },
            [&] (const SaveSessionToKeychainAction & _) -> AccountResult
            {
                return {model, [session = model.session] (auto && context) {
                            SaveSessionToKeychain (session);
                        }};
            },
        },
        action);
}
