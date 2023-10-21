#include "AccountAction.h"

#include "AccountUtils.h"

AccountResult UpdateAccount (AccountModel model, AccountAction action)
{
    return std::visit (
        lager::visitor {
            [&] (const LoadSessionFromKeychainAction & _) -> AccountResult
            {
                auto updated_model = model;
                updated_model.status = AccountModel::Status::kLoading;

                return {updated_model,
                        [] (auto && context)
                        {
                            auto tokens = LoadSessionFromKeychain ();
                            context.dispatch (
                                LoadSessionFromTokensAction {.tokens = tokens.value ()});
                        }};
            },
            [&] (const SaveSessionToKeychainAction & _) -> AccountResult
            {
                if (! model.session.has_value ())
                    return {model, lager::noop};

                return {model, [session = model.session] (auto && context) {
                            SaveSessionToKeychain (session);
                        }};
            },
            [&] (const LoadSessionFromTokensAction & load_session_from_tokens_action)
                -> AccountResult
            {
                auto updated_model = model;
                updated_model.status = AccountModel::Status::kLoading;

                return {updated_model,
                        [tokens = load_session_from_tokens_action.tokens] (auto && context)
                        {
                            try
                            {
                                auto profile = ReadProfileFromIdToken (tokens.id_token);
                                context.dispatch (LoadSessionAction {
                                    .session = {.tokens = tokens, .profile = profile}});
                            }
                            catch (...)
                            {
                            }
                        }};
            },
            [&] (const LoadSessionAction & load_session_action) -> AccountResult
            {
                auto updated_model = model;
                updated_model.status = AccountModel::Status::kAuthenticated;
                updated_model.session = load_session_action.session;
                return {model, lager::noop};
            },
        },
        action);
}
