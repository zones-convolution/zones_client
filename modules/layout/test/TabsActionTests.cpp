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

        auto deps = lager::deps<TabsControllerDelegate &>::with (
            std::reference_wrapper<TabsControllerDelegate> (tabs_controller_delegate_mock));
        auto loop = lager::with_manual_event_loop {};

        std::optional<TabsAction> last_tab_action;
        lager::context<TabsAction, lager::deps<TabsControllerDelegate &>> context (
            [&] (auto && action) -> lager::future
            {
                last_tab_action = action;
                return {};
            },
            loop,
            deps);

        WHEN ("a tab that doesnt exist is loaded")
        {
            tabs_controller_delegate_mock.should_return_success = false;
            LoadTabAction load_tab_action {.tab_name = "test_tab"};
            auto [model, effect] = UpdateTabs (TabsModel {}, load_tab_action);

            THEN ("running the effect has no action")
            {
                REQUIRE (model.current_tab != load_tab_action.tab_name);
                effect (context);
                REQUIRE_FALSE (last_tab_action.has_value ());
            }
        }
        WHEN ("a tab that does exist is loaded")
        {
            tabs_controller_delegate_mock.should_return_success = true;
            LoadTabAction load_tab_action {.tab_name = "test_tab"};
            auto [model, effect] = UpdateTabs (TabsModel {}, load_tab_action);

            THEN ("a tab loaded action is dispatched")
            {
                REQUIRE (model.current_tab != load_tab_action.tab_name);
                effect (context);
                auto tab_loaded_action = std::get<TabLoadedAction> (last_tab_action.value ());
                REQUIRE (tab_loaded_action.tab_name == load_tab_action.tab_name);
            }
        }
        WHEN ("a tab loaded action is dispatched")
        {
            TabLoadedAction tab_loaded_action {.tab_name = "test_tab"};
            auto [model, effect] = UpdateTabs (TabsModel {}, tab_loaded_action);

            REQUIRE (model.current_tab == tab_loaded_action.tab_name);

            //            REQUIRE (effect == lager::noop);
        }
    }
}
