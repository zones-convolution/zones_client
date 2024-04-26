#pragma once
#include "../BrowserModel.h"
#include "browser/BrowserAction.h"
#include "browser/BrowserModel.h"
#include "layout/BannerGrid.h"
#include "look_and_feel/components/DividerComponent.h"
#include "model/Action.h"
#include "model/Model.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <lager/context.hpp>
#include <lager/reader.hpp>
#include <lager/store.hpp>

class HomeViewComponent : public juce::Component
{
public:
    HomeViewComponent (lager::store<BrowserAction, BrowserModel> & browser_store,
                       const lager::reader<Model> & model,
                       lager::context<Action> & context);
    void Update (const HomeView & home_view);
    void resized () override;

private:
    void UpdateIrList ();

    lager::reader<Model> model_;
    lager::context<Action> context_;

    lager::context<BrowserAction> browser_context_;

    lager::reader<IrRepositoryModel> ir_repository_reader_;
    CurrentIrReader ir_reader_;
    UserIrsReader user_irs_reader_;

    std::vector<std::unique_ptr<juce::Component>> cards_;
    BannerGrid card_banner_grid_ {cards_};
    DividerComponent top_divider_;
    juce::Label top_label_ {"User IRs", "User IRs"};
};
