#pragma once
#include "LabelHeightContainer.h"
#include "zones_look_and_feel/LookAndFeel.h"

#include <juce_gui_basics/juce_gui_basics.h>

class DiscreteLevelLabels : public juce::Component
{
public:
    DiscreteLevelLabels (const LabelHeightContainer & label_height_container);

    void resized () override;
    void paint (juce::Graphics & g) override;

private:
    LabelHeightContainer label_height_container_;
};
