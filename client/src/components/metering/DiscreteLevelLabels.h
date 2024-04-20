#pragma once
#include "look_and_feel/LookAndFeel.h"

#include <juce_gui_basics/juce_gui_basics.h>

class DiscreteLevelLabels : public juce::Component
{
public:
    DiscreteLevelLabels ();

    void resized () override;

private:
    std::array<juce::Label, 8> discrete_level_labels_ {juce::Label {"", "0"},
                                                       juce::Label {"", "5"},
                                                       juce::Label {"", "10"},
                                                       juce::Label {"", "15"},
                                                       juce::Label {"", "20"},
                                                       juce::Label {"", "30"},
                                                       juce::Label {"", "40"},
                                                       juce::Label {"", "60"}};
};
