#include "IrWatchController.h"

IrWatchController::IrWatchController (
    IrEngine & ir_engine,
    ProjectIrLoadController & load_controller,
    const lager::reader<ProjectIrRepositoryModel> & project_ir_reader,
    juce::AudioProcessorValueTreeState & parameter_tree)
    : ir_engine_ (ir_engine)
    , load_controller_ (load_controller)
    , current_ir_reader_ (ProjectIrRepositoryModel::CurrentProjectIrReader (project_ir_reader))
    , parameter_tree_ (parameter_tree)
{
    parameter_tree.addParameterListener (ParameterTree::kRoomSizeParameterId, this);
    parameter_tree.addParameterListener (ParameterTree::kReverbTimeParameterId, this);

    WatchCurrentIr ();
    UpdateParametersFromTree ();
}

void IrWatchController::parameterChanged (const juce::String & parameter_id, float new_value)
{
    UpdateParametersFromTree ();
    PerformRender ();
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

void IrWatchController::UpdateParametersFromTree ()
{
    auto room_size_parameter = parameter_tree_.getParameter (ParameterTree::kRoomSizeParameterId);
    current_graph_state_.room_size =
        room_size_parameter->convertFrom0to1 (room_size_parameter->getValue ());

    auto reverb_time_parameter =
        parameter_tree_.getParameter (ParameterTree::kReverbTimeParameterId);
    current_graph_state_.reverb_time =
        reverb_time_parameter->convertFrom0to1 (reverb_time_parameter->getValue ());
}

void IrWatchController::PerformRender ()
{
    ir_engine_.RenderState (current_graph_state_);
}
