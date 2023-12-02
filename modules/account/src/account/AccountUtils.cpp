#include "AccountUtils.h"

#include <jwt-cpp/jwt.h>
#include <keychain/keychain.h>

const std::string kPackage = "com.zones.zones";
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

void SaveSessionToKeychain (const std::optional<AccountModel::Session> & session)
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

std::optional<AccountModel::Tokens> LoadSessionFromKeychain ()
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

const std::string kEmailClaim = "email";
const std::string kUsernameClaim = "cognito:username";
const std::string kGroupsClaim = "cognito:groups";

AccountModel::Profile ReadProfileFromIdToken (const std::string & id_token)
{
    auto decoded = jwt::decode (id_token);
    auto email_claim = decoded.get_payload_claim (kEmailClaim).as_string ();
    // auto username_claim = decoded.get_payload_claim (kUsernameClaim).as_string ();
    // auto groups_claim = decoded.get_payload_claim (kGroupsClaim).as_array ();
    //
    // immer::flex_vector<std::string> groups;
    // for (auto & group : groups_claim)
    //     groups = groups.push_back (group.get<std::string> ());

    immer::flex_vector<std::string> groups;
    std::string username_claim = email_claim;

    return {.email = email_claim, .username = username_claim, .groups = groups};
}