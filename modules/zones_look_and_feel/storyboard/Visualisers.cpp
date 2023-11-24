#include "Visualisers.h"

Visualisers::Visualisers ()
{
    visualisers_label_.setText ("Visualisers", juce::dontSendNotification);
    visualisers_label_.setJustificationType (juce::Justification::centred);

    addAndMakeVisible (visualisers_label_);
    addAndMakeVisible (load_waterfall_);
    addAndMakeVisible (waterfall_component_);
    addAndMakeVisible (divider_);

    waterfall_component_.EnableDebugControls (true);

    load_waterfall_.onClick = [&] ()
    {
        waterfall_chooser_ = std::make_unique<juce::FileChooser> (
            "Please select the waterfall you want to load...",
            juce::File::getSpecialLocation (juce::File::userHomeDirectory),
            "*.wav");

        auto chooser_flags = juce::FileBrowserComponent::openMode |
                             juce::FileBrowserComponent::FileChooserFlags::canSelectFiles;

        waterfall_chooser_->launchAsync (
            chooser_flags,
            [&](const juce::FileChooser & chooser)
            {
                juce::File waterfall_file (chooser.getResult ());

                juce::AudioFormatManager audio_format_manager;
                audio_format_manager.registerBasicFormats ();

                std::unique_ptr<juce::AudioFormatReader> reader (
                    audio_format_manager.createReaderFor (waterfall_file));
                juce::AudioBuffer<float> waterfall_data;
                waterfall_data.setSize (reader->numChannels, reader->lengthInSamples);
                reader->read (&waterfall_data, 0, reader->lengthInSamples, 0, true, true);

                waterfall_component_.SetAudioBlock (waterfall_data);
            });
    };
}

void Visualisers::resized ()
{
    juce::FlexBox header_layout;
    header_layout.flexDirection = juce::FlexBox::Direction::row;
    header_layout.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;
    header_layout.alignItems = juce::FlexBox::AlignItems::center;

    header_layout.items.add (LookAndFeel::LabelFlexItem (visualisers_label_));
    header_layout.items.add (LookAndFeel::ButtonFlexItem (load_waterfall_).withWidth (240.f));

    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;
    layout.alignContent = juce::FlexBox::AlignContent::center;

    layout.items.add (juce::FlexItem (header_layout).withHeight (LookAndFeel::kButtonHeight));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (LookAndFeel::HorizontalDividerFlexItem (divider_));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (waterfall_component_).withFlex (1.f));

    layout.performLayout (getLocalBounds ().toFloat ());
}
