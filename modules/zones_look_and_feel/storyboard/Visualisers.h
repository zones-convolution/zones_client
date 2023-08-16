#pragma once
#include "visualiser/component/WaterfallComponent.h"
#include "zones_look_and_feel/LookAndFeel.h"

#include <juce_gui_basics/juce_gui_basics.h>

class Visualisers : public juce::Component
{
public:
    Visualisers ();
    void resized () override;

private:
    juce::Label visualisers_label_;
    WaterfallComponent waterfall_component_;
};
