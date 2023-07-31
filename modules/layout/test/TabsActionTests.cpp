#include "layout/tabs/TabsAction.h"

#include <catch2/catch_test_macros.hpp>

SCENARIO ("updating tabs model", "[TabsAction]")
{
    GIVEN ("a tabs model")
    {
        WHEN ("a load tab action is dispatched")
        {
            LoadTabAction load_tab_action {.tab_name = "test_tab"};
            auto [model, effect] = UpdateTabs (TabsModel {}, load_tab_action);
            THEN ("the current tab has not changed")
            {
                REQUIRE (model.current_tab != load_tab_action.tab_name);
            }
            THEN ("a load tab effect is created")
            {
            }
        }
    }
}