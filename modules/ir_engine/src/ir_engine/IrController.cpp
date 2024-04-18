#include "IrController.h"

#include "ir_format/IrFormatter.h"
#include "ir_format/io/IrReader.h"

IrController::IrController (IrEngine & ir_engine,
                            juce::AudioProcessorValueTreeState & parameter_tree)
    : ir_engine_ (ir_engine)
    , parameter_tree_ (parameter_tree)
{
    parameter_tree.addParameterListener (ParameterTree::kRoomSizeParameterId, this);
    parameter_tree.addParameterListener (ParameterTree::kReverbTimeParameterId, this);

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

void IrController::LoadIr (const std::filesystem::path & absolute_ir_path,
                           TargetFormat target_format)
{
    IrReader ir_reader;
    auto metadata = ir_reader.ReadIrMetadata (absolute_ir_path);

    IrFormatData ir_format_data {
        .channel_format = metadata.channel_format.value (),
        .position_map = metadata.position_map.value (),
    };

    IrData ir_data;
    CreateTargetIR (absolute_ir_path, ir_format_data, target_format, ir_data);

    std::lock_guard lock {current_graph_state_mutex_};

    current_graph_state_.target_format = target_format;
    current_graph_state_.base_ir_buffer = IrGraphProcessor::BoxedBuffer {ir_data.buffer};
    current_graph_state_.sample_rate = ir_data.sample_rate;
    current_graph_state_.bit_depth = ir_data.bit_depth;
    current_graph_state_.base_ir = absolute_ir_path;

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
}

void IrController::PerformRender ()
{
    ir_engine_.RenderState (current_graph_state_);
}
