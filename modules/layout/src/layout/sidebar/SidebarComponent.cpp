#include "SidebarComponent.h"

SidebarComponent::SidebarComponent (juce::Component & header,
                                    juce::Component & content,
                                    juce::Component & footer)
    : header_ (header)
    , content_ (content)
    , footer_ (footer)
{
    addAndMakeVisible (header_);
    addAndMakeVisible (content_);
    addAndMakeVisible (footer_);
}

void SidebarComponent::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;
    layout.justifyContent = juce::FlexBox::JustifyContent::flexStart;

    layout.items.add (juce::FlexItem (header_).withHeight (60.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (content_).withFlex (1.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (footer_).withHeight (300.f));

    layout.performLayout (getLocalBounds ().toFloat ());
}
