#include "BrowserNavigationComponent.h"

#include "look_and_feel/LookAndFeel.h"

BrowserNavigationComponent::BrowserNavigationComponent (
    lager::store<BrowserAction, BrowserModel> & browser_store,
    const lager::reader<Model> & model,
    lager::context<Action> & context,
    lager::context<TabsAction> & tabs_context)
    : browser_reader_ (browser_store)
    , browser_context_ (browser_store)
    , history_component_ (browser_store)
    , home_view_component_ (browser_store, model, context, tabs_context)
    , zone_view_component_ (context, model)
{
    addAndMakeVisible (history_panel_);
    addAndMakeVisible (content_panel_);

    browser_context_.dispatch (LoadHomeAction {});

    push_home_.onClick = [&] { browser_context_.dispatch (LoadHomeAction {}); };
    push_zone_.onClick = [&] { browser_context_.dispatch (LoadZoneAction {}); };
    push_top_10_.onClick = [&] { browser_context_.dispatch (LoadTop10Action {}); };

    addAndMakeVisible (push_home_);
    addAndMakeVisible (push_zone_);
    addAndMakeVisible (push_top_10_);

    lager::watch (browser_reader_, [&] (auto && model) { UpdateView (model); });
    UpdateView (browser_reader_.get ());
}

void BrowserNavigationComponent::resized ()
{
    juce::FlexBox nav_layout;
    nav_layout.flexDirection = juce::FlexBox::Direction::row;
    nav_layout.justifyContent = juce::FlexBox::JustifyContent::spaceAround;

    nav_layout.items.add (LookAndFeel::MediumIconFlexItem (push_home_));
    nav_layout.items.add (LookAndFeel::MediumIconFlexItem (push_zone_));
    nav_layout.items.add (LookAndFeel::MediumIconFlexItem (push_top_10_));

    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;

    layout.items.add (juce::FlexItem {history_panel_}.withHeight (40.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (content_panel_).withFlex (1.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (nav_layout).withHeight (20.0f));
    layout.items.add (LookAndFeel::kFlexSpacer);

    layout.performLayout (getLocalBounds ().toFloat ());
}

void BrowserNavigationComponent::UpdateView (const BrowserModel & browser_model)
{
    auto history_size = static_cast<int> (browser_model.navigation_stack.size ());
    auto stack_pointer = browser_model.stack_pointer;

    if (stack_pointer + 1 > history_size)
    {
        content_panel_.SetComponent (nullptr);
    }
    else
    {
        auto & current_view = browser_model.navigation_stack [stack_pointer];
        std::visit (
            lager::visitor {
                [&] (const HomeView & view)
                {
                    home_view_component_.Update (view);
                    content_panel_.SetComponent (&home_view_component_);
                },
                [&] (const ZoneView & view)
                {
                    zone_view_component_.Update (view);
                    content_panel_.SetComponent (&zone_view_component_);
                },
                [&] (const Top10View & view)
                {
                    top_10_view_component_.Update (view);
                    content_panel_.SetComponent (&top_10_view_component_);
                },
            },
            current_view.view);
    }

    resized ();
}
