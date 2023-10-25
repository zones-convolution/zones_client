#include "account/AccountAction.h"

#include <catch2/catch_test_macros.hpp>
#include <juce_core/juce_core.h>
#include <lager/context.hpp>
#include <lager/event_loop/manual.hpp>

SCENARIO ("updating account model", "[AccountAction]")
{
    GIVEN ("an empty account model")
    {
        ApiRequestService api_request_service;

        auto deps = AccountDeps::with (std::ref (api_request_service));
        auto loop = lager::with_manual_event_loop {};

        std::optional<AccountAction> last_tab_action;
        lager::context<AccountAction, AccountDeps> context (
            [&] (auto && action) -> lager::future
            {
                last_tab_action = action;
                return {};
            },
            loop,
            deps);

        WHEN ("the session is saved to the keychain")
        {
            SaveSessionToKeychainAction save_session_to_keychain_action;
            auto [model, effect] = UpdateAccount (AccountModel {}, save_session_to_keychain_action);
            effect (context);
        }

        WHEN ("a session is loaded with tokens")
        {
            auto valid_id_token =
                "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJlbWFpbCI6ImVtYWlsQGRvbWFpbi50bGQiLCJjb2duaXRvOnVzZXJuYW1lIjoidXNlcm5hbWUiLCJjb2duaXRvOmdyb3VwcyI6WyJncm91cF8xIiwiZ3JvdXBfMiJdLCJpYXQiOjE2OTc5MDIxNjN9.gI6v1EoufC6cPqGh8BqzoKpWN4RYzXCvY6iey78YGe0";
            LoadSessionFromTokensAction load_session_from_tokens_action {
                .tokens = {.access_token = "access_token",
                           .refresh_token = "refresh_token",
                           .id_token = valid_id_token}};
            auto [model, effect] = UpdateAccount (AccountModel {}, load_session_from_tokens_action);
            effect (context);

            auto load_session_action = std::get<LoadSessionAction> (last_tab_action.value ());
            auto profile = load_session_action.session.profile;
            REQUIRE (profile.username == "username");
        }
    }
}