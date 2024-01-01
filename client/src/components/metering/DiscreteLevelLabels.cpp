#include "DiscreteLevelLabels.h"

DiscreteLevelLabels::DiscreteLevelLabels (LabelHeightContainer & label_height_container)
    : label_height_container_ (label_height_container)
{
}

void DiscreteLevelLabels::resized ()
{
    //    juce::FlexBox layout;
    //    layout.flexDirection = juce::FlexBox::Direction::column;
    //    layout.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;
    //
    //    for (auto & discrete_level_label : discrete_level_labels_)
    //        layout.items.add (LookAndFeel::LabelFlexItem (discrete_level_label));
    //
    //    layout.performLayout (getLocalBounds ().toFloat ());
}

void DiscreteLevelLabels::paint (juce::Graphics & g)
{
    auto width = getWidth ();
    auto height = getHeight ();
    // setLookAndFeel (LookAndFeel);
    g.setColour (juce::Colours::white);

    //

    auto labels = label_height_container_.getLabelsAsString ();
    auto label_heights = label_height_container_.getHeightsWithinBounds (getLocalBounds ());

    for (auto i = 0; i < labels.size (); ++i)
    {
        g.drawFittedText (
            labels [i], 0, label_heights [i], width, 2, juce::Justification::centred, 1);
        //  g.fillRect (0, label_height, width, kBarHeight);
    }
}
