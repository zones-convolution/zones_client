#include "AudioEngine.h"

AudioEngine::AudioEngine (CommandQueue::VisitorQueue & command_queue,
                          juce::AudioProcessorValueTreeState & parameter_tree,
                          zones::ConvolutionEngine & convolution_engine,
                          const juce::AudioProcessor & processor)
    : processor_ (processor)
    , command_queue_ (command_queue)
    , parameter_tree_ (parameter_tree)
    , convolution_engine_ (convolution_engine)
{
    parameter_tree.addParameterListener (ParameterTree::kDryWetMixParameterId, this);
    parameter_tree.addParameterListener (ParameterTree::kOutputGainParameterId, this);
    parameter_tree.addParameterListener (ParameterTree::kInputGainParameterId, this);
}

zones::Convolver::ConvolverSpec
AudioEngine::CreateConvolverSpecForState (const IrGraphState & ir_graph_state)
{
    switch (ir_graph_state.target_format)
    {
        case TargetFormat::kMono:
            return zones::Convolver::ConvolverSpec {.input_routing = {0}, .output_routing = {0}};
        case TargetFormat::kStereo:
            return zones::Convolver::ConvolverSpec {.input_routing = {0, 1},
                                                    .output_routing = {0, 1}};
        case TargetFormat::kTrueStereo:
            return zones::Convolver::ConvolverSpec {.input_routing = {0, 0, 1, 1},
                                                    .output_routing = {0, 1, 0, 1}};
        case TargetFormat::kFoa:
            return zones::Convolver::ConvolverSpec {.input_routing = {0, 1, 2, 3},
                                                    .output_routing = {0, 1, 2, 3}};
    }
}

void AudioEngine::RenderFinished (IrGraphState state, IrGraphProcessor::BoxedBuffer render_result)
{
    if (render_result->getNumChannels () == 0 || render_result->getNumSamples () == 0)
        return;

    auto output_channel_set = processor_.getBusesLayout ().getMainOutputChannelSet ();
    if (IsTargetSupported (output_channel_set, state.target_format))
        convolution_engine_.LoadIR (*render_result, CreateConvolverSpecForState (state));
    else
        jassertfalse;
}

void AudioEngine::parameterChanged (const juce::String & parameterID, float newValue)
{
    auto dry_wet_mix_parameter =
        parameter_tree_.getParameter (ParameterTree::kDryWetMixParameterId);
    auto input_gain_parameter = parameter_tree_.getParameter (ParameterTree::kInputGainParameterId);
    auto output_gain_parameter =
        parameter_tree_.getParameter (ParameterTree::kOutputGainParameterId);

    command_queue_.PushCommand (CommandQueue::UpdateParameters {
        .dry_wet_mix = dry_wet_mix_parameter->convertFrom0to1 (dry_wet_mix_parameter->getValue ()),
        .input_gain = input_gain_parameter->convertFrom0to1 (input_gain_parameter->getValue ()),
        .output_gain =
            output_gain_parameter->convertFrom0to1 (output_gain_parameter->getValue ())});
}
