#include "IrWatchController.h"

IrWatchController::IrWatchController (
    IrEngine & ir_engine,
    ProjectIrLoadController & load_controller,
    const lager::reader<ProjectIrRepositoryModel> & project_ir_reader,
    const lager::reader<BoxedParameterModel> & parameter_model_reader)
    : ir_engine_ (ir_engine)
    , load_controller_ (load_controller)
    , current_ir_reader_ (ProjectIrRepositoryModel::CurrentProjectIrReader (project_ir_reader))
    , parameter_model_reader_ (parameter_model_reader)
{
    current_graph_state_.room_size = 1.f;
    current_graph_state_.reverb_time = 1.f;

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
                      current_graph_state_.reverb_time = parameter_model->reverb_time;
                      PerformRender ();
                  });
}

void IrWatchController::PerformRender ()
{
    ir_engine_.RenderState (current_graph_state_);
}
