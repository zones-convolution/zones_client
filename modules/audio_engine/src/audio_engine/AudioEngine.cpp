#include "AudioEngine.h"

AudioEngine::AudioEngine (CommandQueue::VisitorQueue & command_queue,
                          juce::AudioProcessorValueTreeState & parameter_tree)
    : command_queue_ (command_queue)
    , parameter_tree_ (parameter_tree)
{
    parameter_tree.addParameterListener (ParameterTree::kDryWetMixParameterId, this);
    parameter_tree.addParameterListener (ParameterTree::kOutputGainParameterId, this);
    parameter_tree.addParameterListener (ParameterTree::kInputGainParameterId, this);
}

void AudioEngine::LoadIr (const IrData & ir_data)
{
    auto handover_ir_buffer = new juce::AudioBuffer<float> ();
    handover_ir_buffer->makeCopyOf (ir_data.buffer);

    command_queue_.PushCommand (
        CommandQueue::LoadIr {.ir_buffer = handover_ir_buffer, .sample_rate = ir_data.sample_rate});
}

void AudioEngine::RenderFinished (IrGraphState state, IrGraphProcessor::BoxedBuffer render_result)
{
    LoadIr ({
        .buffer = render_result,
        .sample_rate = state.sample_rate,
        .bit_depth = state.bit_depth,
    });
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
