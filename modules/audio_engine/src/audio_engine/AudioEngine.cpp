#include "AudioEngine.h"

AudioEngine::AudioEngine (CommandQueue & command_queue,
                          const ProjectIrLoadController & load_controller,
                          lager::reader<CurrentProjectIrOptional> current_ir_reader)
    : command_queue_ (command_queue)
    , current_ir_reader_ (current_ir_reader)
    , load_controller_ (load_controller)
{
    lager::watch (current_ir_reader_,
                  [&] (const CurrentProjectIrOptional & current_project_ir)
                  {
                      if (current_project_ir.has_value ())
                      {
                          auto current_ir_data = load_controller_.GetCurrentProjectIrData ();
                          ir_data_ = std::make_unique<IrData> (
                              IrData {.buffer = juce::AudioBuffer<float> (),
                                      .sample_rate = current_ir_data->sample_rate,
                                      .bit_depth = current_ir_data->bit_depth});
                          ir_data_->buffer.makeCopyOf (current_ir_data->buffer);
                          command_queue_.LoadIr (ir_data_.get ());
                      }
                  });
}
