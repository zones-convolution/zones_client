#include "PluginEditor.h"

#include "PluginProcessor.h"
#include "look_and_feel/LookAndFeel.h"

AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (
    AudioPluginAudioProcessor & processor,
    ProcessorContainer & processor_container)
    : AudioProcessorEditor (&processor)
    , processor_ (processor)
    , processor_container_ (processor_container)
    , model_ (processor_container.store_)
    , context_ (processor_container.store_)
    , editor_ (processor_container.parameter_tree_,
               processor_container.thread_pool_,
               processor_container.player_controller_)
    , ir_engine_ (processor_container.ir_engine_)

{
    setResizable (true, true);
    setResizeLimits (kWindowMinimumWidth,
                     static_cast<int> (kWindowMinimumWidth * kPreferredAspectRatio),
                     kWindowMaxWidth,
                     static_cast<int> (kWindowMaxWidth * kPreferredAspectRatio));

    setSize (1280, 600);

    auto & ir_engine_listeners = ir_engine_.GetListeners ();
    ir_engine_listeners.add (&editor_);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor ()
{
    auto & ir_engine_listeners = ir_engine_.GetListeners ();
    ir_engine_listeners.remove (&editor_);
}

void AudioPluginAudioProcessorEditor::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::row;

    layout.performLayout (getLocalBounds ().toFloat ().reduced (LookAndFeel::kPadding));
}

void AudioPluginAudioProcessorEditor::paint (juce::Graphics & graphics)
{
    graphics.fillAll (juce::Colour::fromRGB (23, 26, 29));
}
