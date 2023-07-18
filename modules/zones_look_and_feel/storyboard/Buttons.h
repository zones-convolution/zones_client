#pragma once
#include "zones_look_and_feel/LookAndFeel.h"

#include <juce_gui_basics/juce_gui_basics.h>

class Buttons : public juce::Component
{
public:
    Buttons ();
    void resized () override;

private:
    juce::Label buttons_label_;
    juce::TextButton button_primary_ {"Primary Button"};
    juce::TextButton button_secondary_ {"Secondary Button"};
    juce::TextButton button_disabled_ {"Disabled Button"};
};
