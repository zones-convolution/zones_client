#pragma once
#include "../LookAndFeel.h"

#include <juce_gui_basics/juce_gui_basics.h>

class IconComponent : public juce::Component
{
public:
    void paint (juce::Graphics & g) override;
    void SetIcon (const juce::String & icon);

private:
    juce::String icon_;
};
