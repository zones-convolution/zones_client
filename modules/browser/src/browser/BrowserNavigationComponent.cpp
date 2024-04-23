#include "BrowserNavigationComponent.h"

#include "look_and_feel/LookAndFeel.h"

BrowserHistoryComponent::BrowserHistoryComponent (
    lager::store<BrowserAction, BrowserModel> & browser_store)
    : browser_reader_ (browser_store)
    , browser_context_ (browser_store)
{
    addAndMakeVisible (navigate_back_);
    addAndMakeVisible (navigate_forward_);
    addAndMakeVisible (current_view_);

    navigate_back_.onClick = [&]
    { browser_context_.dispatch (JumpAction {.position = browser_reader_->stack_pointer - 1}); };

    navigate_forward_.onClick = [&]
    { browser_context_.dispatch (JumpAction {.position = browser_reader_->stack_pointer + 1}); };

    auto update_component = [&] (const BrowserModel & browser_model)
    {
        auto history_size = static_cast<int> (browser_model.navigation_stack.size ());
        auto stack_pointer = browser_model.stack_pointer;
        if (stack_pointer + 1 > history_size)
        {
            current_view_.setText ("View Out Of Bounds", juce::dontSendNotification);
        }
        else
        {
            auto & current_view = browser_model.navigation_stack [stack_pointer];

            std::visit (
                lager::visitor {
                    [&] (const HomeView &)
                    {
                        current_view_.setText ("Home View: " + current_view.id.toDashedString (),
                                               juce::dontSendNotification);
                    },
                    [&] (const ZoneView &)
                    {
                        current_view_.setText ("Zone View: " + current_view.id.toDashedString (),
                                               juce::dontSendNotification);
                    },

                    [&] (const Top10View &)
                    {
                        current_view_.setText ("Top 10 View: " + current_view.id.toDashedString (),
                                               juce::dontSendNotification);
                    },
                },
                current_view.view);
        }

        navigate_forward_.setEnabled (stack_pointer < history_size - 1);
        navigate_back_.setEnabled (stack_pointer > 0);

        resized ();
    };

    lager::watch (browser_reader_, update_component);
    update_component (browser_reader_.get ());
}

void BrowserHistoryComponent::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::row;
    layout.alignItems = juce::FlexBox::AlignItems::center;

    layout.items.add (LookAndFeel::MediumIconFlexItem (navigate_back_));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (LookAndFeel::MediumIconFlexItem (navigate_forward_));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (LookAndFeel::LabelFlexItem (current_view_));

    layout.performLayout (getLocalBounds ().toFloat ());
}

BrowserNavigationComponent::BrowserNavigationComponent (
    lager::store<BrowserAction, BrowserModel> & browser_store)
    : browser_reader_ (browser_store)
    , browser_context_ (browser_store)
    , history_component_ (browser_store)
{
    addAndMakeVisible (history_panel_);

    push_home_.onClick = [&] { browser_context_.dispatch (LoadHomeAction {}); };
    push_zone_.onClick = [&] { browser_context_.dispatch (LoadZoneAction {}); };
    push_top_10_.onClick = [&] { browser_context_.dispatch (LoadTop10Action {}); };

    addAndMakeVisible (push_home_);
    addAndMakeVisible (push_zone_);
    addAndMakeVisible (push_top_10_);
}

void BrowserNavigationComponent::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;

    layout.items.add (juce::FlexItem {history_panel_}.withHeight (40.f));

    layout.items.add (LookAndFeel::MediumIconFlexItem (push_home_));
    layout.items.add (LookAndFeel::MediumIconFlexItem (push_zone_));
    layout.items.add (LookAndFeel::MediumIconFlexItem (push_top_10_));

    layout.performLayout (getLocalBounds ().toFloat ());
}
