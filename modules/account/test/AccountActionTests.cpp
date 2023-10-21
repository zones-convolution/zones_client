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
    }
}