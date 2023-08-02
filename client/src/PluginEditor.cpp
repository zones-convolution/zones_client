#include "PluginEditor.h"

#include "PluginProcessor.h"

AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (
    AudioPluginAudioProcessor & processor,
    ProcessorContainer & processor_container)
    : AudioProcessorEditor (&processor)
    , processor_ (processor)
    , model_ (processor_container.store_)
    , context_ (processor_container.store_)
    , editor_ (parameter_context_)
    , browser_ (Model::ProjectIrRepositoryReader (processor_container.store_),
                project_ir_repository_context_)

{
    juce::LookAndFeel::setDefaultLookAndFeel (&look_and_feel_);

    setResizable (true, true);
    setResizeLimits (kWindowMinimumWidth,
                     static_cast<int> (kWindowMinimumWidth * kPreferredAspectRatio),
                     kWindowMaxWidth,
                     static_cast<int> (kWindowMaxWidth * kPreferredAspectRatio));

    setSize (1280, 600);

    tabs_controller_.AddTab ("browser", browser_panel_);
    tabs_controller_.AddTab ("editor", editor_panel_);
    tabs_controller_.AddTab ("settings", settings_panel_);

    addAndMakeVisible (tabs_component_);
    addAndMakeVisible (sidebar_component_);

    store_.dispatch (LoadTabAction {.tab_name = "editor"});
}

void AudioPluginAudioProcessorEditor::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::row;

    layout.items.add (juce::FlexItem (sidebar_component_).withWidth (300.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (tabs_component_).withFlex (1.f));

    layout.performLayout (getLocalBounds ().toFloat ().reduced (LookAndFeel::kPadding));
}

void AudioPluginAudioProcessorEditor::paint (juce::Graphics & graphics)
{
    graphics.fillAll (juce::Colour::fromRGB (23, 26, 29));
}