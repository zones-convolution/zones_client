#include "IOPanel.h"

const std::string IOPanel::kIOPanelKey = "io_panel";
const std::string IOPanel::kDryWetKey = "dry_wet_parameter";
const std::string IOPanel::kInputGainKey = "input_gain_parameter";
const std::string IOPanel::kOutputGainKey = "output_gain_parameter";

IOPanel::IOPanel ()
{
    addAndMakeVisible (io_label_);
    addAndMakeVisible (dry_wet_mix_slider_);
    addAndMakeVisible (dry_wet_label_);
    addAndMakeVisible (input_gain_slider_);
    addAndMakeVisible (input_gain_label_);
    addAndMakeVisible (output_gain_slider_);
    addAndMakeVisible (output_gain_label_);

    io_label_.setText (juce::translate (kIOPanelKey), juce::dontSendNotification);

    dry_wet_label_.setText (juce::translate (kDryWetKey), juce::dontSendNotification);
    dry_wet_label_.setJustificationType (juce::Justification::centred);
    dry_wet_mix_slider_.setRange ({0.f, 1.f}, 0);
    dry_wet_mix_slider_.setPopupDisplayEnabled (true, true, getTopLevelComponent ());
    dry_wet_mix_slider_.onValueChange = [&]
    {
        //        realtime_parameter_context_.dispatch (
        //            UpdateDryWetMix (static_cast<float> (dry_wet_mix_slider_.getValue ())));
    };

    input_gain_label_.setText (juce::translate (kInputGainKey), juce::dontSendNotification);
    input_gain_label_.setJustificationType (juce::Justification::centred);
    input_gain_slider_.setRange ({0.f, 2.f}, 0);
    input_gain_slider_.setPopupDisplayEnabled (true, true, getTopLevelComponent ());
    input_gain_slider_.onValueChange = [&]
    {
        //        realtime_parameter_context_.dispatch (
        //            UpdateInputGain (static_cast<float> (input_gain_slider_.getValue ())));
    };

    output_gain_label_.setText (juce::translate (kOutputGainKey), juce::dontSendNotification);
    output_gain_label_.setJustificationType (juce::Justification::centred);
    output_gain_slider_.setRange ({0.f, 2.f}, 0);
    output_gain_slider_.setPopupDisplayEnabled (true, true, getTopLevelComponent ());
    output_gain_slider_.onValueChange = [&]
    {
        //        realtime_parameter_context_.dispatch (
        //            UpdateOutputGain (static_cast<float> (output_gain_slider_.getValue ())));
    };
}

void IOPanel::resized ()
{
    static const auto kTrack = juce::Grid::TrackInfo (juce::Grid::Fr (1));

    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;

    layout.items.add (LookAndFeel::LabelFlexItem (io_label_));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem ().withFlex (1.f));

    layout.performLayout (getLocalBounds ().toFloat ());

    juce::Grid grid_layout;
    auto labels_height = LookAndFeel::GetLabelBounds (dry_wet_label_).getHeight ();
    auto labels_track = juce::Grid::TrackInfo (juce::Grid::Px (labels_height));
    grid_layout.templateRows = {kTrack, labels_track};
    grid_layout.templateColumns = {kTrack, kTrack, kTrack};
    grid_layout.setGap (juce::Grid::Px (LookAndFeel::kGap));

    grid_layout.items.add (juce::GridItem (dry_wet_mix_slider_));
    grid_layout.items.add (juce::GridItem (input_gain_slider_));
    grid_layout.items.add (juce::GridItem (output_gain_slider_));
    grid_layout.items.add (juce::GridItem (dry_wet_label_));
    grid_layout.items.add (juce::GridItem (input_gain_label_));
    grid_layout.items.add (juce::GridItem (output_gain_label_));

    grid_layout.performLayout (layout.items [2].currentBounds.toNearestInt ());
}
