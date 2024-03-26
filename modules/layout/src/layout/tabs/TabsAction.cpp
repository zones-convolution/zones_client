#include "TabsAction.h"

TabsResult UpdateTabs (TabsModel tabs_model, TabsAction tabs_action)
{
    return std::visit (
        lager::visitor {
            [&] (const LoadTabAction & load_tab_action) -> TabsResult
            {
                if (tabs_model.current_tab == load_tab_action.tab_name)
                    return {tabs_model, lager::noop};

                return {tabs_model,
                        [tab_name = load_tab_action.tab_name] (auto && context)
                        {
                            auto & tabs_controller_delegate =
                                lager::get<TabsControllerDelegate> (context);
                            if (tabs_controller_delegate.LoadTab (tab_name, true))
                                context.dispatch (TabLoadedAction {.tab_name = tab_name});
                        }};
            },
            [&] (const TabLoadedAction & tab_loaded_action) -> TabsResult
            {
                tabs_model.current_tab = tab_loaded_action.tab_name;
                return {tabs_model, lager::noop};
            },

        },
        tabs_action);
}
