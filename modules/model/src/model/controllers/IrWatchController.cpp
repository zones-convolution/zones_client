#include "IrWatchController.h"

IrWatchController::IrWatchController (AudioEngine & audio_engine,
                                      IrEngine & ir_engine,
                                      ProjectIrLoadController & load_controller,
                                      lager::reader<ProjectIrRepositoryModel> project_ir_reader,
                                      lager::reader<BoxedParameterModel> parameter_model_reader)
    : audio_engine_ (audio_engine)
    , ir_engine_ (ir_engine)
    , load_controller_ (load_controller)
    , current_ir_reader_ (project_ir_reader.zoom (
          lager::lenses::attr (&ProjectIrRepositoryModel::current_project_ir)))
    , parameter_model_reader_ (parameter_model_reader)
{
    WatchCurrentIr ();
    WatchParameterModel ();
}

void IrWatchController::WatchCurrentIr ()
{
    lager::watch (current_ir_reader_,
                  [&] (const CurrentProjectIrOptional & current_ir)
                  {
                      if (current_ir.has_value ())
                      {
                          auto current_ir_data = load_controller_.GetCurrentProjectIrData ();
                          current_graph_state_.base_ir_buffer =
                              IrGraphProcessor::BoxedBuffer {current_ir_data->buffer};
                          current_graph_state_.sample_rate = current_ir_data->sample_rate;
                          current_graph_state_.bit_depth = current_ir_data->bit_depth;
                          current_graph_state_.base_ir = current_ir.value ();
                          PerformRender ();
                      }
                  });
}

void IrWatchController::WatchParameterModel ()
{
    lager::watch (parameter_model_reader_,
                  [&] (const BoxedParameterModel & parameter_model)
                  {
                      current_graph_state_.room_size = parameter_model->room_size;
                      PerformRender ();
                  });
}

void IrWatchController::PerformRender ()
{
    DBG ("RENDER START");
    ir_engine_.RenderState (
        current_graph_state_,
        [&,
         sample_rate = current_graph_state_.sample_rate,
         bit_depth = current_graph_state_.bit_depth] (IrGraphProcessor::BoxedBuffer render_result)
        {
            DBG ("RENDER DONE - FINISHED");
            if (render_result->getNumChannels () == 0 || render_result->getNumChannels () == 0)
                return;
            DBG ("RENDER DONE - LOAD");
            audio_engine_.LoadIr ({
                .buffer = render_result,
                .sample_rate = sample_rate,
                .bit_depth = bit_depth,
            });
        });
}
