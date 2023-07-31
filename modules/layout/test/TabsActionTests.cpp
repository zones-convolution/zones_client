#include "layout/tabs/TabsAction.h"

#include <catch2/catch_test_macros.hpp>
#include <future>
#include <lager/context.hpp>
#include <lager/event_loop/manual.hpp>
#include <lager/store.hpp>

// lager::detail::dispatcher<lager::as_actions_t<TabsAction>> dispatcher {
//    [] (auto && act) { return lager::promise::invalid (); }};

struct TabsControllerDelegateMock : public TabsControllerDelegate
{
    bool LoadTab (const std::string & tab_name, bool animate) override
    {
        return false;
    }
};

SCENARIO ("updating tabs model", "[TabsAction]")
{
    GIVEN ("a tabs model")
    {
        TabsControllerDelegateMock tabs_controller_delegate_mock;
        auto store = lager::make_store<TabsAction> (
            TabsModel {},
            lager::with_manual_event_loop {},
            lager::with_reducer (UpdateTabs),
            lager::with_deps (std::ref (tabs_controller_delegate_mock)));

        WHEN ("a load tab action is dispatched")
        {
            LoadTabAction load_tab_action {.tab_name = "test_tab"};
            store.dispatch (load_tab_action);
            THEN ("the current tab has not changed")
            {
                REQUIRE (store->current_tab != load_tab_action.tab_name);
            }
            THEN ("a load tab effect is created")
            {
            }
        }
    }
}