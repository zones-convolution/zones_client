#pragma once

#include "../LookAndFeel.h"
#include "juce_gui_basics/juce_gui_basics.h"

class PanelComponent : public juce::Component
{
public:
    explicit PanelComponent (juce::Component & child, bool apply_content_rounding = false);

    void paint (juce::Graphics & g) override;

    void resized () override;

    void paintOverChildren (juce::Graphics & g) override;

private:
    bool apply_content_rounding_;
    juce::Component & child_;
};
