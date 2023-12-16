#include "AccountUtils.h"

#include <jwt-cpp/jwt.h>
#include <keychain/keychain.h>

const std::string kPackage = "com.zones.zones";
const std::string kUser = "Admin";

const std::string kAccessTokenKey = "access_token";
const std::string kRefreshTokenKey = "refresh_token";
const std::string kIdTokenKey = "id_token";

const std::string kTokensKey = "tokens";

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
    auto composite_key_builder = new juce::DynamicObject ();
    composite_key_builder->setProperty (juce::Identifier (kAccessTokenKey),
                                        juce::String (session->tokens.access_token));

    composite_key_builder->setProperty (juce::Identifier (kRefreshTokenKey),
                                        juce::String (session->tokens.refresh_token));

    composite_key_builder->setProperty (juce::Identifier (kIdTokenKey),
                                        juce::String (session->tokens.id_token));

    juce::var composite (
        composite_key_builder); // Takes ownership of dynamic, really dislike this...
    auto composite_key = juce::JSON::toString (composite).toStdString ();

    keychain::Error error {};
    if (session.has_value ())
        SetKeychainKey (kTokensKey, composite_key, error);
    else
        DeleteKeychainKey (kTokensKey, error);
}

std::optional<AccountModel::Tokens> LoadSessionFromKeychain ()
{
    keychain::Error error {};
    auto tokens = GetKeychainKey (kTokensKey, error);
    if (error)
        return std::nullopt;

    auto composite = juce::JSON::fromString (tokens);
    auto composite_key = composite.getDynamicObject ();

    auto access_token =
        composite_key->getProperty (juce::Identifier (kAccessTokenKey)).toString ().toStdString ();
    auto refresh_token =
        composite_key->getProperty (juce::Identifier (kRefreshTokenKey)).toString ().toStdString ();
    auto id_token =
        composite_key->getProperty (juce::Identifier (kIdTokenKey)).toString ().toStdString ();

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
    auto username_claim = decoded.get_payload_claim (kUsernameClaim).as_string ();
    auto groups_claim = decoded.get_payload_claim (kGroupsClaim).as_array ();

    immer::flex_vector<std::string> groups;
    for (auto & group : groups_claim)
        groups = groups.push_back (group.get<std::string> ());

    return {.email = email_claim, .username = username_claim, .groups = groups};
}