#include "BrowserHistoryComponent.h"

static std::string GetTitleForView (const BrowserView & browser_view)
{
    return std::visit (
        lager::visitor {
            [&] (const HomeView &) -> std::string { return "Browse"; },
            [&] (const ZoneView & zone_view) -> std::string
            { return zone_view.ir_metadata.name ? *zone_view.ir_metadata.name : "Zone"; },
            [&] (const Top10View &) -> std::string { return "Top 10"; },
        },
        browser_view);
}

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
            current_view_.setText (GetTitleForView (current_view.view), juce::dontSendNotification);
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
