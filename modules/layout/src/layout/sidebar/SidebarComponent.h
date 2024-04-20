#pragma once
#include "look_and_feel/LookAndFeel.h"

#include <juce_gui_basics/juce_gui_basics.h>

class SidebarComponent : public juce::Component
{
public:
    SidebarComponent (juce::Component & header,
                      juce::Component & content,
                      juce::Component & footer);
    void resized () override;

private:
    juce::Component & header_;
    juce::Component & content_;
    juce::Component & footer_;
};
