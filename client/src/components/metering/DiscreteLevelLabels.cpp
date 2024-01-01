#include "DiscreteLevelLabels.h"

DiscreteLevelLabels::DiscreteLevelLabels (LabelHeightContainer & label_height_container)
    : label_height_container_ (label_height_container)
{
    auto string_labels = label_height_container_.getLabelsAsString ();

    for (auto i = 0; i < string_labels.size (); ++i)
    {
        auto label = std::make_unique<juce::Label> ();
        addAndMakeVisible (*label);
        label->setText (string_labels [i], juce::dontSendNotification);
        label->setColour (juce::Label::textColourId, juce::Colours::white);
        labels_.push_back (std::move (label));
    }

    auto t = std::make_unique<juce::Label> ();
    addAndMakeVisible (*t);
    t->setText ("a", juce::dontSendNotification);
    t->setColour (juce::Label::textColourId, juce::Colours::orange);
    test_l_ = std::move (t);
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
    auto width = getWidth ();
    auto height = getHeight ();

    auto bounds = getLocalBounds ();
    bounds.reduce (0, LookAndFeel::kMeterMargin);
    auto label_heights = label_height_container_.getHeightsWithinBounds (bounds);

    auto font_height = (int) labels_ [0]->getFont ().getHeight ();
    for (auto i = 0; i < labels_.size (); ++i)
    {
        // g.drawFittedText (
        //    labels [i], 0, label_heights [i], width, 2, juce::Justification::centred, 1);
        //  g.fillRect (0, label_height, width, kBarHeight);

        // auto font_width = (int) labels_ [i]->getFont ().getStringWidth (labels_ [i]->getText ());
        juce::Rectangle<int> label_bounds (0, label_heights [i] - 1, width, font_height);
        labels_ [i]->setBounds (label_bounds);
    }

    // test_l_->setBoundsToFit (bounds, juce::Justification::horizontallyJustified, false);
    //  test_l_->setBounds (bounds);
}

void DiscreteLevelLabels::paint (juce::Graphics & g)
{
    //    auto width = getWidth ();
    //    auto height = getHeight ();
    //    g.setColour (juce::Colours::white);
    //
    //    auto bounds = getLocalBounds ();
    //    bounds.reduce (0, LookAndFeel::kMeterMargin);
    //    auto label_heights = label_height_container_.getHeightsWithinBounds (bounds);
    //
    //    auto labels = label_height_container_.getLabelsAsString ();
    //
    //    for (auto i = 0; i < labels.size (); ++i)
    //    {
    //        g.drawFittedText (
    //            labels [i], 0, label_heights [i], width, 2, juce::Justification::centred, 1);
    //        //  g.fillRect (0, label_height, width, kBarHeight);
    //    }
}
