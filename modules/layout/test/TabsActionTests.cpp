#include "layout/WithJuceEventLoop.h"
#include "layout/tabs/TabsAction.h"

#include <catch2/catch_test_macros.hpp>
#include <future>
#include <juce_core/juce_core.h>
#include <lager/context.hpp>
#include <lager/event_loop/manual.hpp>
#include <lager/store.hpp>
// lager::detail::dispatcher<lager::as_actions_t<TabsAction>> dispatcher {
//    [] (auto && act) { return lager::promise::invalid (); }};

//        lager::invoke_reducer<TabsEffect::deps_t, decltype (UpdateTabs), TabsModel,
//        TabsAction> (
//            UpdateTabs,
//            TabsModel {},
//            LoadTabAction {},
//            [&] (TabsEffect & effect)
//            {
//                auto deps = lager::with_deps (
//                    std::reference_wrapper<TabsControllerDelegate>
//                    (tabs_controller_delegate_mock));
//                effect (lager::make_deps (deps));
//            },
//            [&] { jassert (false); });

struct TabsControllerDelegateMock : public TabsControllerDelegate
{
    bool LoadTab (const std::string & tab_name, bool animate) override
    {
        last_tab_name = tab_name;
        return should_return_success;
    }

    std::string last_tab_name;
    bool should_return_success = false;

    ~TabsControllerDelegateMock () override = default;
};

SCENARIO ("updating tabs model", "[TabsAction]")
{
    GIVEN ("an empty tabs model")
    {
        TabsControllerDelegateMock tabs_controller_delegate_mock;

        auto store = lager::make_store<TabsAction> (
            TabsModel {},
            WithJuceEventLoop {},
            lager::with_reducer (&UpdateTabs),
            lager::with_deps (
                std::reference_wrapper<TabsControllerDelegate> (tabs_controller_delegate_mock)));

        WHEN ("a load tab action is dispatched")
        {
            tabs_controller_delegate_mock.should_return_success = false;
            LoadTabAction load_tab_action {.tab_name = "test_tab"};
            store.dispatch (load_tab_action);

            THEN ("the current tab has not changed")
            {
                REQUIRE (tabs_controller_delegate_mock.last_tab_name == load_tab_action.tab_name);
                REQUIRE (store->current_tab != load_tab_action.tab_name);
            }
        }

        WHEN ("a load tab action is dispatched")
        {
            tabs_controller_delegate_mock.should_return_success = true;
            LoadTabAction load_tab_action {.tab_name = "test_tab"};
            store.dispatch (load_tab_action);

            THEN ("the current tab has updated")
            {
                REQUIRE (tabs_controller_delegate_mock.last_tab_name == load_tab_action.tab_name);
                REQUIRE (store->current_tab == load_tab_action.tab_name);
            }
        }
    }
}