#include "AccountAction.h"

#include "AccountUtils.h"

AccountResult UpdateAccount (AccountModel model, AccountAction action)
{
    return std::visit (
        lager::visitor {
            [&] (const LoadSessionFromKeychainAction & _) -> AccountResult
            {
                auto updated_model = model;
                updated_model.account_status = AccountModel::AccountStatus::kLoading;

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
                updated_model.account_status = AccountModel::AccountStatus::kLoading;

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
                updated_model.account_status = AccountModel::AccountStatus::kAuthenticated;
                updated_model.session = load_session_action.session;
                return {updated_model, lager::noop};
            },
            [&] (const DiscoverOidcAction & discover_oidc_action) -> AccountResult
            {
                auto updated_model = model;
                updated_model.oidc_status = AccountModel::OidcStatus::kLoading;
                updated_model.oidc_discovery = std::nullopt;
                return {
                    updated_model,
                    [base_url = discover_oidc_action.base_url] (
                        const lager::context<AccountAction> & context)
                    {
                        context.loop ().async (
                            [context, base_url] ()
                            {
                                auto req = OidcApi::DiscoverRequest (base_url);
                                auto res = req.get ();

                                if (res.status_code == 200)
                                    context.dispatch (DiscoverOidcSuccessAction {
                                        .discover_success = OidcApi::ReadDiscoverSuccess (res)});
                                else
                                    context.dispatch (DiscoverOidcFailedAction {});
                            });
                    }};
            },
            [&] (const DiscoverOidcSuccessAction & discover_oidc_success_action) -> AccountResult
            {
                auto updated_model = model;
                updated_model.oidc_status = AccountModel::OidcStatus::kSuccess;
                updated_model.oidc_discovery = discover_oidc_success_action.discover_success;
                return {updated_model, lager::noop};
            },
            [&] (const DiscoverOidcFailedAction & discover_oidc_failed_action) -> AccountResult
            {
                auto updated_model = model;
                updated_model.oidc_status = AccountModel::OidcStatus::kFailed;
                updated_model.oidc_discovery = std::nullopt;
                return {updated_model, lager::noop};
            },
            [&] (const RefreshTokenAction & refresh_token_action) -> AccountResult
            {
                if (model.is_refreshing_token || ! model.oidc_discovery.has_value ())
                    return {model, lager::noop};

                auto updated_model = model;
                updated_model.is_refreshing_token = true;

                return {
                    updated_model,
                    [model] (const lager::context<AccountAction> & context)
                    {
                        context.loop ().async (
                            [context, model] ()
                            {
                                auto token_endpoint =
                                    model.oidc_discovery->token_endpoint.toStdString ();
                                auto refresh_token = model.session->tokens.refresh_token;

                                auto req =
                                    OidcApi::RefreshTokenRequest (token_endpoint, refresh_token);
                                auto res = req.get ();
                                if (res.status_code == 200)
                                {
                                    auto refresh_token_success =
                                        OidcApi::ReadRefreshTokenSuccess (res);
                                    context.dispatch (RefreshTokenSuccessAction {
                                        .tokens = {
                                            .access_token =
                                                refresh_token_success.access_token.toStdString (),
                                            .id_token =
                                                refresh_token_success.id_token.toStdString (),
                                            .refresh_token = refresh_token_success.refresh_token
                                                                 .toStdString ()}});
                                }
                                else
                                {
                                    context.dispatch (RefreshTokenFailedAction {});
                                }
                            });
                    }};
            },
            [&] (const RefreshTokenSuccessAction & refresh_token_success_action) -> AccountResult
            {
                auto updated_model = model;
                updated_model.session->tokens = refresh_token_success_action.tokens;
                updated_model.is_refreshing_token = false;

                return {updated_model, [] (const lager::context<AccountAction> & context) {
                            context.dispatch (SaveSessionToKeychainAction {});
                        }};
            },
            [&] (const RefreshTokenFailedAction & refresh_token_failed) -> AccountResult
            {
                auto updated_model = model;
                updated_model.is_refreshing_token = false;
                return {updated_model, lager::noop};
            }},

        action);
}
