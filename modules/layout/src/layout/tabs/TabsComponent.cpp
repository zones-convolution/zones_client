#include "TabsComponent.h"

void TabsComponent::DisplayTab (juce::Component & tab)
{
    if (current_tab_.has_value ())
        removeChildComponent (&current_tab_->get ());

    current_tab_ = std::reference_wrapper<juce::Component> (tab);
    addAndMakeVisible (tab);
    resized ();
}

void TabsComponent::resized ()
{
    if (! current_tab_.has_value ())
        return;

    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;
    layout.justifyContent = juce::FlexBox::JustifyContent::center;

    layout.items.add (juce::FlexItem {current_tab_->get ()}.withFlex (1.f));
    layout.performLayout (getLocalBounds ().toFloat ());
}

TabsComponent::~TabsComponent ()
{
    if (current_tab_.has_value ())
        removeChildComponent (&current_tab_->get ());
}
