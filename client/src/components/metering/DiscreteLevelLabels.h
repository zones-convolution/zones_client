#pragma once
#include "zones_look_and_feel/LookAndFeel.h"

#include <juce_gui_basics/juce_gui_basics.h>

class DiscreteLevelLabels : public juce::Component
{
public:
    DiscreteLevelLabels ();

    void resized () override;

private:
    std::array<juce::Label, 8> discrete_level_labels_ {};
};
