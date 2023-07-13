#include "AudioEngine.h"

AudioEngine::AudioEngine (CommandQueue & command_queue,
                          const ProjectIrLoadController & load_controller)
    : command_queue_ (command_queue)
    , load_controller_ (load_controller)
{
}

void AudioEngine::LoadIr (const IrData & ir_data)
{
    ir_data_ = std::make_unique<IrData> (IrData {.buffer = juce::AudioBuffer<float> (),
                                                 .sample_rate = ir_data.sample_rate,
                                                 .bit_depth = ir_data.bit_depth});
    ir_data_->buffer.makeCopyOf (ir_data.buffer);
    command_queue_.LoadIr (ir_data_.get ());
}
