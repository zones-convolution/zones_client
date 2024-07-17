#include "ProcessorContainer.h"

#include "model/ParameterTree.h"

ProcessorContainer::ProcessorContainer (juce::AudioProcessor & audio_processor)
    : parameter_tree_state_ (audio_processor,
                             nullptr,
                             ParameterTree::kParameterTreeIdentifier,
                             CreateParameterLayout ())

    , graph_ (input_graph_metering_,
              output_graph_metering_,
              convolution_engine_,
              notification_queue_,
              parameter_tree_)
    , audio_engine_ (command_queue_,
                     notification_queue_,
                     convolution_engine_,
                     player_controller_,
                     audio_processor)

{
    command_queue_.SetVisitor (&graph_);
    notification_queue_.SetVisitor (&audio_engine_);

    RegisterIrEngineListeners ();
}

void ProcessorContainer::Prepare (double sampleRate,
                                  int samplesPerBlock,
                                  juce::AudioProcessor::BusesLayout layout)
{
    graph_.prepare (
        juce::dsp::ProcessSpec {sampleRate,
                                static_cast<juce::uint32> (samplesPerBlock),
                                static_cast<juce::uint32> (layout.getMainOutputChannels ())});

    auto output_channel_set = layout.getMainOutputChannelSet ();
    auto state = ir_controller_.GetCurrentGraphState ();
    if (! IsTargetSupported (output_channel_set, state.target_format))
        convolution_engine_.Clear ();
    load_controller_.UpdateValidTargetFormats (GetTargetFormatsForChannelSet (output_channel_set));
}

void ProcessorContainer::RegisterIrEngineListeners ()
{
    auto & ir_engine_listeners = ir_engine_.GetListeners ();
    ir_engine_listeners.add (&audio_engine_);
}

juce::AudioProcessorValueTreeState::ParameterLayout ProcessorContainer::CreateParameterLayout ()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    parameter_tree_ = ParameterTree::CreateParameterTree (layout);
    return layout;
}
