#include "SidebarLayout.h"

namespace zones
{
SidebarLayout::SidebarLayout (juce::Component & sidebar, juce::Component & content)
{
    addAndMakeVisible (sidebar);
    addAndMakeVisible (content);

    m_flexBox.flexDirection = juce::FlexBox::Direction::row;

    m_flexBox.items.add (juce::FlexItem (sidebar).withMinWidth (m_sidebarMinimumWidth));
    m_flexBox.items.add (juce::FlexItem (content).withFlex (1.f));
}

void zones::SidebarLayout::resized ()
{
    m_flexBox.performLayout (getLocalBounds ());
}

}