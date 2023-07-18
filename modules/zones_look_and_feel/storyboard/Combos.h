#pragma once
#include "zones_look_and_feel/LookAndFeel.h"

#include <juce_gui_basics/juce_gui_basics.h>

class Combos : public juce::Component
{
public:
    Combos ();
    void resized () override;

private:
    juce::Label combos_label_;

    juce::ComboBox combo_box_small_;
    juce::ComboBox combo_box_large_;

    juce::ComboBox combo_box_disabled_;
};
