#include "IrWatchController.h"

IrWatchController::IrWatchController (IrEngine & ir_engine,
                                      ProjectIrLoadController & load_controller,
                                      lager::reader<CurrentProjectIrOptional> current_ir_reader)
    : ir_engine_ (ir_engine)
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

                          ir_engine_.RenderState (
                              {.base_ir = current_ir.value (), .base_ir_buffer = boxed_ir},
                              [] (const auto & render_result) { DBG ("RENDER FINISHED!"); });
                      }
                  });
}
