#include "AudioEngine.h"

AudioEngine::AudioEngine (CommandQueue::VisitorQueue & command_queue,
                          lager::reader<BoxedRealtimeParameterModel> realtime_parameter_reader)
    : command_queue_ (command_queue)
    , realtime_parameter_reader_ (realtime_parameter_reader)
{
    lager::watch (realtime_parameter_reader_,
                  [&] (const BoxedRealtimeParameterModel & parameter_model)
                  {
                      command_queue_.PushCommand (CommandQueue::UpdateParameters {
                          .dry_wet_mix = parameter_model->dry_wet_mix,
                          .input_gain = parameter_model->input_gain,
                          .output_gain = parameter_model->output_gain});
                  });
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
