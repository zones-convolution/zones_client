#pragma once

#include "BrowserAction.h"
#include "BrowserModel.h"
#include "components/BrowserHistoryComponent.h"
#include "components/HomeViewComponent.h"
#include "components/Top10ViewComponent.h"
#include "components/ZoneViewComponent.h"
#include "layout/tabs/TabsAction.h"
#include "look_and_feel/BoxIcons.h"
#include "look_and_feel/components/IconButton.h"
#include "look_and_feel/components/PanelComponent.h"
#include "model/Action.h"
#include "model/Model.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <lager/context.hpp>
#include <lager/reader.hpp>
#include <lager/store.hpp>

class BrowserNavigationComponent : public juce::Component
{
public:
    explicit BrowserNavigationComponent (lager::store<BrowserAction, BrowserModel> & browser_store,
                                         const lager::reader<Model> & model,
                                         lager::context<Action> & context,
                                         lager::context<TabsAction> & tabs_context);
    ~BrowserNavigationComponent () override = default;

    void resized () override;

private:
    void UpdateView (const BrowserModel & browser_model);

    lager::reader<BrowserModel> browser_reader_;
    lager::context<BrowserAction> browser_context_;

    BrowserHistoryComponent history_component_;
    PanelComponent history_panel_ {history_component_};
    PanelComponent content_panel_ {};

    HomeViewComponent home_view_component_;
    ZoneViewComponent zone_view_component_;
    Top10ViewComponent top_10_view_component_;

    IconButton push_home_ {"Push Home", BoxIcons::kBxHome};
    IconButton push_zone_ {"Push Zone", BoxIcons::kBxAlarm};
    IconButton push_top_10_ {"Push Top 10", BoxIcons::kBxAnchor};
};
