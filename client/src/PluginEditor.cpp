#include "PluginEditor.h"

#include "PluginProcessor.h"

AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (
    AudioPluginAudioProcessor & processor,
    ProcessorContainer & processor_container)
    : AudioProcessorEditor (&processor)
    , processor_ (processor)
    , model_ (processor_container.store_)
    , context_ (processor_container.store_)
    , editor_ (processor_container.parameter_tree_)
    , browser_ (Model::ProjectIrRepositoryReader (processor_container.store_),
                project_ir_repository_context_)
    , sidebar_footer_ (processor_container.store_ [&Model::project_ir_repository_model]
                                                  [&ProjectIrRepositoryModel::current_project_ir],
                       processor_container.input_graph_metering_,
                       processor_container.output_graph_metering_)

{
    juce::LookAndFeel::setDefaultLookAndFeel (&look_and_feel_);

    setResizable (true, true);
    setResizeLimits (kWindowMinimumWidth,
                     static_cast<int> (kWindowMinimumWidth * kPreferredAspectRatio),
                     kWindowMaxWidth,
                     static_cast<int> (kWindowMaxWidth * kPreferredAspectRatio));

    setSize (1280, 600);

    tabs_controller_.AddTab ("browser", browser_panel_);
    tabs_controller_.AddTab ("editor", editor_);
    tabs_controller_.AddTab ("settings", settings_panel_);

    addAndMakeVisible (tabs_component_);
    addAndMakeVisible (sidebar_component_);

    store_.dispatch (LoadTabAction {.tab_name = "editor"});

    auto & ir_engine_listeners = processor_container.ir_engine_.GetListeners ();
    ir_engine_listeners.add (&editor_);
}

void AudioPluginAudioProcessorEditor::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::row;

    layout.items.add (juce::FlexItem (sidebar_component_).withWidth (250.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (tabs_component_).withFlex (1.f));

    layout.performLayout (getLocalBounds ().toFloat ().reduced (LookAndFeel::kPadding));
}

void AudioPluginAudioProcessorEditor::paint (juce::Graphics & graphics)
{
    graphics.fillAll (juce::Colour::fromRGB (23, 26, 29));
}