#pragma once
#include "zones_look_and_feel/LookAndFeel.h"

#include <juce_gui_basics/juce_gui_basics.h>

class ClippingIndicator : public juce::ToggleButton
{
public:
    void paint (juce::Graphics & g) override;
    void setFill (bool is_clipping);

private:
    bool is_clipping_ = false;
};