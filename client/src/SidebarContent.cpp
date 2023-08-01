#include "SidebarContent.h"

SidebarContent::SidebarContent (lager::context<TabsAction> tabs_context)
    : tabs_context_ (tabs_context)
{
    addAndMakeVisible (browse_button_);
    addAndMakeVisible (edit_button_);
    addAndMakeVisible (settings_button_);

    browse_button_.onClick = [&] { tabs_context_.dispatch (LoadTabAction {.tab_name = "browse"}); };
    edit_button_.onClick = [&] { tabs_context_.dispatch (LoadTabAction {.tab_name = "edit"}); };
    settings_button_.onClick = [&]
    { tabs_context_.dispatch (LoadTabAction {.tab_name = "settings"}); };
}

void SidebarContent::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;

    layout.items.add (juce::FlexItem {browse_button_}.withHeight (40.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem {edit_button_}.withHeight (40.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem {settings_button_}.withHeight (40.f));

    layout.performLayout (getLocalBounds ().toFloat ());
}
