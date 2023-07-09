#include "PluginEditor.h"

#include "PluginProcessor.h"

AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (
    AudioPluginAudioProcessor & processor,
    const lager::reader<ProjectIrRepositoryModel> & model,
    lager::context<ProjectIrRepositoryAction> & context)
    : AudioProcessorEditor (&processor)
    , processor_ (processor)
    , model_ (model)
    , context_ (context)
{
    juce::LookAndFeel::setDefaultLookAndFeel (&look_and_feel_);

    setResizable (true, true);
    setResizeLimits (kWindowMinimumWidth,
                     static_cast<int> (kWindowMinimumWidth * kPreferredAspectRatio),
                     kWindowMaxWidth,
                     static_cast<int> (kWindowMaxWidth * kPreferredAspectRatio));

    addAndMakeVisible (project_import_panel_);
}

void AudioPluginAudioProcessorEditor::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::row;
    layout.alignItems = juce::FlexBox::AlignItems::center;

    layout.items.add (juce::FlexItem (project_import_panel_).withFlex (1.f).withHeight (200.f));

    layout.performLayout (getLocalBounds ().toFloat ().reduced (LookAndFeel::kPadding));
}

void AudioPluginAudioProcessorEditor::paint (juce::Graphics & graphics)
{
    graphics.fillAll (juce::Colour::fromRGB (23, 26, 29));
}