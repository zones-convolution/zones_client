#include "DiscreteLevelLabels.h"

DiscreteLevelLabels::DiscreteLevelLabels (LabelContainer & label_height_container)
    : label_height_container_ (label_height_container)
{
    auto string_labels = label_height_container_.getLabelsAsString ();

    for (auto & string_label : string_labels)
    {
        auto label = std::make_unique<juce::Label> ();
        addAndMakeVisible (*label);
        label->setText (string_label, juce::dontSendNotification);
        label->setColour (juce::Label::textColourId, juce::Colours::white);
        labels_.push_back (std::move (label));
    }
}

void DiscreteLevelLabels::resized ()
{
    auto width = getWidth ();

    auto bounds = getLocalBounds ();
    bounds.reduce (0, LookAndFeel::kMeterMargin);
    auto label_heights = label_height_container_.getHeightsWithinBounds (bounds);

    auto font_height = (int) labels_ [0]->getFont ().getHeight ();
    for (auto i = 0; i < labels_.size (); ++i)
    {
        juce::Rectangle<int> label_bounds (0, label_heights [i] - 1, width, font_height);
        labels_ [i]->setBounds (label_bounds);
    }
}
