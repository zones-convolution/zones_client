#include "AccountAction.h"

#include <keychain/keychain.h>

const std::string kPackage = "com.zones";
const std::string kUser = "Admin";

const std::string kAccessTokenKey = "access_token";
const std::string kRefreshTokenKey = "refresh_token";
const std::string kIdTokenKey = "id_token";

static void
SetKeychainKey (const std::string & key, const std::string & value, keychain::Error & error)
{
    keychain::setPassword (kPackage, key, kUser, value, error);
}

static std::string GetKeychainKey (const std::string & key, keychain::Error & error)
{
    return keychain::getPassword (kPackage, key, kUser, error);
}

static void DeleteKeychainKey (const std::string & key, keychain::Error & error)
{
    keychain::deletePassword (kPackage, key, kUser, error);
}

static void SaveSessionToKeychain (const std::optional<AccountModel::Session> & session)
{
    keychain::Error error {};

    if (session.has_value ())
    {
        SetKeychainKey (kAccessTokenKey, session->tokens.access_token, error);
        SetKeychainKey (kRefreshTokenKey, session->tokens.refresh_token, error);
        SetKeychainKey (kIdTokenKey, session->tokens.id_token, error);
    }
    else
    {
        DeleteKeychainKey (kAccessTokenKey, error);
        DeleteKeychainKey (kRefreshTokenKey, error);
        DeleteKeychainKey (kIdTokenKey, error);
    }
}

static std::optional<AccountModel::Tokens> LoadSessionFromKeychain ()
{
    keychain::Error error {};
    auto access_token = GetKeychainKey (kAccessTokenKey, error);
    if (error)
        return std::nullopt;
    auto refresh_token = GetKeychainKey (kRefreshTokenKey, error);
    if (error)
        return std::nullopt;
    auto id_token = GetKeychainKey (kIdTokenKey, error);
    if (error)
        return std::nullopt;

    return AccountModel::Tokens {
        .access_token = access_token, .refresh_token = refresh_token, .id_token = id_token};
}

AccountResult UpdateAccount (AccountModel model, AccountAction action)
{
    return std::visit (
        lager::visitor {
            [&] (const LoadSessionFromKeychainAction & _) -> AccountResult
            {
                auto updated_model = model;
                updated_model.status = AccountModel::Status::kLoading;

                return {updated_model,
                        [] (auto && context) { auto tokens = LoadSessionFromKeychain (); }};
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
