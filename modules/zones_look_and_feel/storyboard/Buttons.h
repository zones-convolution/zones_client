#pragma once
#include "zones_look_and_feel/BoxIcons.h"
#include "zones_look_and_feel/LookAndFeel.h"
#include "zones_look_and_feel/components/IconTextButton.h"

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

    IconTextButton icon_text_button_ {"Icon Button", BoxIcons::kBxAbacus};
};
