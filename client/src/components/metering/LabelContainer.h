#pragma once
#include "juce_gui_basics/juce_gui_basics.h"

#include <string>
#include <vector>

class LabelContainer
{
public:
    LabelContainer (std::vector<float> meter_label_list);

    std::vector<int> getHeightsWithinBounds (juce::Rectangle<int> local_bounds);
    std::vector<std::string> getLabelsAsString ();

private:
    float skewValue (float linear_value);

    std::vector<float> meter_label_list_;
};
