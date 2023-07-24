#include "AudioEngine.h"

AudioEngine::AudioEngine (CommandQueue::VisitorQueue & command_queue,
                          lager::reader<BoxedParameterModel> parameter_model_reader)
    : command_queue_ (command_queue)
    , parameter_model_reader_ (parameter_model_reader)
{
    lager::watch (parameter_model_reader_,
                  [&] (const BoxedParameterModel & parameter_model)
                  {
                      command_queue_.PushCommand (CommandQueue::UpdateParameters {
                          .dry_wet_mix = parameter_model->dry_wet_mix});
                  });
}

void AudioEngine::LoadIr (const IrData & ir_data)
{
    ir_data_ = std::make_unique<IrData> (IrData {.buffer = juce::AudioBuffer<float> (),
                                                 .sample_rate = ir_data.sample_rate,
                                                 .bit_depth = ir_data.bit_depth});
    ir_data_->buffer.makeCopyOf (ir_data.buffer);
    command_queue_.PushCommand (CommandQueue::LoadIr {.ir_data = ir_data_.get ()});
}
