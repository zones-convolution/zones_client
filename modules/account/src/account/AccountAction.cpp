#include "AccountAction.h"

#include "AccountUtils.h"
#include "api/DeviceApi.h"

AccountResult UpdateAccount (AccountModel model, AccountAction action)
{
    return std::visit (
        lager::visitor {
            [&] (const ClearSessionAction & _) -> AccountResult
            {
                auto updated_model = model;
                updated_model.session = std::nullopt;
                updated_model.account_status = AccountModel::AccountStatus::kUnauthenticated;
                updated_model.device_flow = std::nullopt;
                updated_model.device_flow_status = AccountModel::DeviceFlowStatus::kIdle;
                return {updated_model, [] (auto && context) {
                            context.dispatch (SaveSessionToKeychainAction {});
                        }};
            },
            [&] (const LoadSessionFromKeychainAction & _) -> AccountResult
            {
                auto updated_model = model;
                updated_model.account_status = AccountModel::AccountStatus::kLoading;

                return {updated_model,
                        [] (auto && context)
                        {
                            auto tokens = LoadSessionFromKeychain ();
                            if (tokens.has_value ())
                            {
                                context.dispatch (
                                    LoadSessionFromTokensAction {.tokens = tokens.value ()});
                            }
                        }};
            },
            [&] (const SaveSessionToKeychainAction & _) -> AccountResult
            {
                SaveSessionToKeychain (model.session);
                return {model, lager::noop};
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
                return {updated_model,
                        [] (auto & context) { context.dispatch (SaveSessionToKeychainAction {}); }};
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
            },
            [&] (const DeviceCodeAction & device_code_action) -> AccountResult
            {
                if (model.device_flow_status != AccountModel::DeviceFlowStatus::kIdle)
                    return {model, lager::noop};

                auto updated_model = model;
                updated_model.device_flow_status = AccountModel::DeviceFlowStatus::kFetchingCode;
                return {
                    updated_model,
                    [] (const lager::context<AccountAction> & context)
                    {
                        context.loop ().async (
                            [context] ()
                            {
                                const auto base_url = "https://oauth2.googleapis.com";

                                auto req = DeviceApi::DeviceCodeRequest (
                                    base_url,
                                    "687494975680-4m8b1mala896lep7b5hghlpigo1bsjvf.apps.googleusercontent.com",
                                    "email profile");
                                auto res = req.get ();

                                if (res.status_code == 200)
                                {
                                    auto device_code_success =
                                        DeviceApi::ReadDeviceCodeSuccess (res);
                                    context.dispatch (DeviceCodeSuccessAction {
                                        .device_code_success = device_code_success});
                                }
                                else
                                {
                                    context.dispatch (DeviceCodeFailedAction {});
                                }
                            });
                    }};
            },
            [&] (const DeviceCodeSuccessAction & device_code_success_action) -> AccountResult
            {
                auto updated_model = model;
                juce::URL ("https://accounts.google.com/device").launchInDefaultBrowser ();
                updated_model.device_flow = device_code_success_action.device_code_success;
                return {updated_model,
                        [device_code = device_code_success_action.device_code_success.device_code] (
                            const lager::context<AccountAction> & context) {
                            context.dispatch (
                                PollDeviceTokenAction {.device_code = device_code.toStdString ()});
                        }};
            },
            [&] (const DeviceCodeFailedAction & device_code_failed_action) -> AccountResult
            {
                auto updated_model = model;
                updated_model.device_flow_status = AccountModel::DeviceFlowStatus::kIdle;
                return {updated_model, lager::noop};
            },
            [&] (const PollDeviceTokenAction & poll_device_token_action) -> AccountResult
            {
                auto updated_model = model;
                if (! model.device_flow.has_value () ||
                    model.device_flow->device_code.toStdString () !=
                        poll_device_token_action.device_code)
                {
                    updated_model.device_flow_status = AccountModel::DeviceFlowStatus::kIdle;
                    return {updated_model, lager::noop};
                }

                updated_model.device_flow_status = AccountModel::DeviceFlowStatus::kPollingToken;
                auto device_flow = model.device_flow.value ();
                return {
                    updated_model,
                    [device_flow,
                     poll_device_token_action] (const lager::context<AccountAction> & context)
                    {
                        context.loop ().async (
                            [context, device_flow, poll_device_token_action] ()
                            {
                                auto device_code = device_flow.device_code.toStdString ();
                                auto poll_interval = device_flow.interval;

                                const auto base_url = "https://oauth2.googleapis.com";

                                auto req = DeviceApi::DeviceTokenRequest (
                                    base_url,
                                    device_code,
                                    "687494975680-4m8b1mala896lep7b5hghlpigo1bsjvf.apps.googleusercontent.com",
                                    "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",
                                    "email profile");
                                auto res = req.get ();

                                if (res.status_code == 200)
                                {
                                    auto tokens = DeviceApi::ReadDeviceTokenSuccess (res);
                                    context.dispatch (LoadSessionFromTokensAction {
                                        .tokens = {
                                            .access_token = tokens.access_token.toStdString (),
                                            .id_token = tokens.id_token.toStdString (),
                                            .refresh_token = tokens.refresh_token.toStdString ()}});
                                }
                                else
                                {
                                    std::this_thread::sleep_for (
                                        std::chrono::seconds (poll_interval));
                                    context.dispatch (poll_device_token_action);
                                }
                            });
                    }};
            }},
        action);
}
