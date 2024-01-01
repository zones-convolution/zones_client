#include "LabelContainer.h"

#include <sstream>

LabelContainer::LabelContainer (std::vector<float> meter_label_list)
{
    meter_label_list_ = meter_label_list;
}

float LabelContainer::skewValue (float linear_value)
{
    return log10 (1 + (9 * linear_value));
}

std::vector<int> LabelContainer::getHeightsWithinBounds (juce::Rectangle<int> local_bounds)
{
    auto height = local_bounds.getHeight ();

    std::vector<int> label_heights = {};

    for (auto & db_value : meter_label_list_)
    {
        auto linear_value = std::pow (10, db_value / 20);
        auto skewed_value = skewValue (linear_value);
        auto label_height = height - (skewed_value * height);
        label_heights.push_back (label_height);
    }
    return label_heights;
}

std::vector<std::string> LabelContainer::getLabelsAsString ()
{
    std::vector<std::string> string_label_list;

    for (auto & label : meter_label_list_)
    {
        std::stringstream stream;
        stream << std::fixed << std::setprecision (0) << label;
        string_label_list.push_back (stream.str ());
    }
    return string_label_list;
}
