#include "IrWatchController.h"

IrWatchController::IrWatchController (AudioEngine & audio_engine,
                                      IrEngine & ir_engine,
                                      ProjectIrLoadController & load_controller,
                                      lager::reader<CurrentProjectIrOptional> current_ir_reader)
    : audio_engine_ (audio_engine)
    , ir_engine_ (ir_engine)
    , load_controller_ (load_controller)
    , current_ir_reader_ (current_ir_reader)
{
    lager::watch (current_ir_reader_,
                  [&] (const CurrentProjectIrOptional & current_ir)
                  {
                      if (current_ir.has_value ())
                      {
                          auto current_ir_data = load_controller_.GetCurrentProjectIrData ();
                          auto boxed_ir = IrGraphProcessor::BoxedBuffer {current_ir_data->buffer};
                          auto sample_rate = current_ir_data->sample_rate;
                          auto bit_depth = current_ir_data->bit_depth;
                          ir_engine_.RenderState ({.base_ir = current_ir.value (),
                                                   .base_ir_buffer = boxed_ir,
                                                   .sample_rate = sample_rate,
                                                   .bit_depth = bit_depth,
                                                   .room_size = .5f},
                                                  [&, sample_rate, bit_depth] (
                                                      IrGraphProcessor::BoxedBuffer render_result)
                                                  {
                                                      audio_engine_.LoadIr ({
                                                          .buffer = render_result,
                                                          .sample_rate = sample_rate,
                                                          .bit_depth = bit_depth,
                                                      });
                                                  });
                      }
                  });
}
