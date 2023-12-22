#include "LabelHeightContainer.h"

std::vector<int> LabelHeightContainer::getHeightsWithinBounds (juce::Rectangle<int> local_bounds)
{
    auto height = local_bounds.getHeight ();

    std::vector<int> label_heights = {};

    for (auto & db_value : meter_label_list_)
    {
        auto linear_value = std::pow (10, db_value / 20);
        auto skewed_value = log10 (1 + (9 * linear_value));
        auto label_height = height - (skewed_value * height);
        label_heights.push_back (label_height);
    }
    return label_heights;
}
