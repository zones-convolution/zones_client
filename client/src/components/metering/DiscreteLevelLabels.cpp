#include "DiscreteLevelLabels.h"

DiscreteLevelLabels::DiscreteLevelLabels ()
{
    for (auto & discrete_level_label : discrete_level_labels_)
    {
        discrete_level_label.setJustificationType (juce::Justification::topRight);
        addAndMakeVisible (discrete_level_label);
    }
}

void DiscreteLevelLabels::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;
    layout.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;

    for (auto & discrete_level_label : discrete_level_labels_)
        layout.items.add (LookAndFeel::LabelFlexItem (discrete_level_label));

    layout.performLayout (getLocalBounds ().toFloat ());
}
