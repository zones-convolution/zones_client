#pragma once
#include "juce_gui_basics/juce_gui_basics.h"

#include <vector>

class LabelHeightContainer
{
public:
    std::vector<int> getHeightsWithinBounds (juce::Rectangle<int> local_bounds);

private:
    std::vector<float> meter_label_list_ {-3, -6, -9};
};
