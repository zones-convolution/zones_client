#include "IrController.h"

#include "format/IrFormatter.h"

IrController::IrController (IrEngine & ir_engine,
                            juce::AudioProcessorValueTreeState & parameter_tree)
    : ir_engine_ (ir_engine)
    , parameter_tree_ (parameter_tree)
{
    parameter_tree.addParameterListener (ParameterTree::kRoomSizeParameterId, this);
    parameter_tree.addParameterListener (ParameterTree::kReverbTimeParameterId, this);
    parameter_tree.addParameterListener (ParameterTree::kResamplerParameterId, this);

    UpdateParametersFromTree ();
}

void IrController::parameterChanged (const juce::String & parameter_id, float new_value)
{
    UpdateParametersFromTree ();
    startTimer (kDebounceTimeMs);
}

void IrController::timerCallback ()
{
    PerformRender ();
    stopTimer ();
}

void IrController::LoadIr (const IrSelection & ir_selection)
{
    auto & zone_path = ir_selection.zone.path_attribute;
    auto & ir = ir_selection.ir;
    auto target_format = ir_selection.target_format;

    IrFormatData ir_format_data {
        .channel_format = ir.channel_format.value (),
        .position_map = ir.position_map.value (),
    };

    auto ir_path = zone_path / ir.relative_path;

    IrData ir_data;
    CreateTargetIR (ir_path, ir_format_data, target_format, ir_data);

    std::lock_guard lock {current_graph_state_mutex_};

    current_graph_state_.target_format = target_format;
    current_graph_state_.base_ir_buffer = IrGraphProcessor::BoxedBuffer {ir_data.buffer};
    current_graph_state_.sample_rate = ir_data.sample_rate;
    current_graph_state_.bit_depth = ir_data.bit_depth;
    current_graph_state_.base_ir = ir_path;

    PerformRender ();
}

void IrController::UpdateParametersFromTree ()
{
    std::lock_guard lock {current_graph_state_mutex_};

    auto room_size_parameter = parameter_tree_.getParameter (ParameterTree::kRoomSizeParameterId);
    current_graph_state_.room_size =
        room_size_parameter->convertFrom0to1 (room_size_parameter->getValue ());

    auto reverb_time_parameter =
        parameter_tree_.getParameter (ParameterTree::kReverbTimeParameterId);
    current_graph_state_.reverb_time =
        reverb_time_parameter->convertFrom0to1 (reverb_time_parameter->getValue ());

    auto resampler_parameter = parameter_tree_.getParameter (ParameterTree::kResamplerParameterId);
    current_graph_state_.resampler_ratio =
        resampler_parameter->convertFrom0to1 (resampler_parameter->getValue ());
}

void IrController::PerformRender ()
{
    ir_engine_.RenderState (current_graph_state_);
}

IrGraphState IrController::GetCurrentGraphState ()
{
    return current_graph_state_;
}
