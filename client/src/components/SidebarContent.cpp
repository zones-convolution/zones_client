#include "SidebarContent.h"

SidebarContent::SidebarContent (lager::context<TabsAction> tabs_context)
    : tabs_context_ (tabs_context)
{
    addAndMakeVisible (browse_button_);
    addAndMakeVisible (edit_button_);
    addAndMakeVisible (settings_button_);

    browse_button_.onClick = [&]
    { tabs_context_.dispatch (LoadTabAction {.tab_name = "browser"}); };
    edit_button_.onClick = [&] { tabs_context_.dispatch (LoadTabAction {.tab_name = "editor"}); };
    settings_button_.onClick = [&]
    { tabs_context_.dispatch (LoadTabAction {.tab_name = "settings"}); };
}

void SidebarContent::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;

    layout.items.add (LookAndFeel::ButtonFlexItem (browse_button_));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (LookAndFeel::ButtonFlexItem (edit_button_));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (LookAndFeel::ButtonFlexItem (settings_button_));

    layout.performLayout (getLocalBounds ().toFloat ());
}
