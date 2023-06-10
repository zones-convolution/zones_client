#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

namespace zones
{
class SidebarLayout : public juce::Component
{
public:
    SidebarLayout (juce::Component & sidebar, juce::Component & content);

private:
    static constexpr int m_sidebarMinimumWidth = 200;

    juce::FlexBox m_flexBox;

    void resized () override;
};
}
