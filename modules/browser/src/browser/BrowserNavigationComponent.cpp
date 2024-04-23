#include "BrowserNavigationComponent.h"

#include "look_and_feel/LookAndFeel.h"

BrowserHistoryComponent::BrowserHistoryComponent ()
{
    addAndMakeVisible (navigate_back_);
    addAndMakeVisible (navigate_forward_);
    addAndMakeVisible (current_view_);

    current_view_.setText ("Current View", juce::dontSendNotification);
    navigate_back_.setEnabled (false);
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
    : browser_model_ (browser_store)
    , browser_context_ (browser_store)
{
    addAndMakeVisible (history_panel_);
}

void BrowserNavigationComponent::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;

    layout.items.add (juce::FlexItem {history_panel_}.withHeight (40.f));

    layout.performLayout (getLocalBounds ().toFloat ());
}
