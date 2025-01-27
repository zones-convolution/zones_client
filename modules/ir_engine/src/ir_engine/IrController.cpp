#include "IrController.h"

#include "format/IrFormatter.h"

IrController::IrController (IrEngine & ir_engine, ParameterTree & parameter_tree)
    : ir_engine_ (ir_engine)
    , parameter_tree_ (parameter_tree)
{
    parameter_tree_.reverb_time_parameter->addListener (this);
    parameter_tree_.resampler_parameter->addListener (this);
    parameter_tree_.pre_delay_parameter->addListener (this);
    parameter_tree_.trim_parameter->addListener (this);
    parameter_tree_.attack_parameter->addListener (this);

    UpdateParametersFromTree ();
}

void IrController::parameterValueChanged (int parameter_index, float new_value)
{
    UpdateParametersFromTree ();
    startTimer (kDebounceTimeMs);
}

void IrController::timerCallback ()
{
    PerformRender ();
    stopTimer ();
}

void IrController::Prepare (const juce::dsp::ProcessSpec & spec)
{
    // block size is handled separately by preferences controller
    std::lock_guard lock {current_graph_state_mutex_};
    if (current_graph_state_.sample_rate != spec.sampleRate)
    {
        current_graph_state_.sample_rate = spec.sampleRate;
        startTimer (kDebounceTimeMs);
    }
}

static std::filesystem::path GetZonesDataDirectory ()
{
    return juce::File::getSpecialLocation (
               juce::File::SpecialLocationType::userApplicationDataDirectory)
        .getChildFile ("zones")
        .getChildFile ("zones")
        .getFullPathName ()
        .toStdString ();
}

void IrController::LoadIr (const IrSelection & ir_selection)
{
    auto & zone = ir_selection.zone;
    std::filesystem::path zone_path;

    if (zone.zone_type == ZoneType::kWeb)
        zone_path = GetZonesDataDirectory () / *zone.zone_id;
    else
        zone_path = ir_selection.zone.path_attribute.value ();

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
    current_graph_state_.base_ir_sample_rate = ir_data.sample_rate;
    current_graph_state_.base_ir_bit_depth = ir_data.bit_depth;
    current_graph_state_.base_ir = ir_path.string ();

    PerformRender ();
}

void IrController::UpdateParametersFromTree ()
{
    std::lock_guard lock {current_graph_state_mutex_};

    auto reverb_time_parameter = parameter_tree_.reverb_time_parameter;
    current_graph_state_.reverb_time_norm =
        reverb_time_parameter->convertTo0to1 (*reverb_time_parameter);

    current_graph_state_.resampler = *parameter_tree_.resampler_parameter;

    current_graph_state_.pre_delay_ms = *parameter_tree_.pre_delay_parameter;

    auto trim_parameter = parameter_tree_.trim_parameter;
    current_graph_state_.trim_norm = trim_parameter->convertTo0to1 (*trim_parameter);

    auto attack_parameter = parameter_tree_.attack_parameter;
    current_graph_state_.attack_norm = attack_parameter->convertTo0to1 (*attack_parameter);
}

void IrController::PerformRender ()
{
    ir_engine_.RenderState (current_graph_state_);
}

IrGraphState IrController::GetCurrentGraphState ()
{
    return current_graph_state_;
}

void IrController::parameterGestureChanged (int parameterIndex, bool gestureIsStarting)
{
}

void IrController::internalBlockSizeValueChanged (const int new_block_size)
{
    std::lock_guard lock {current_graph_state_mutex_};
    current_graph_state_.convolver_block_size = new_block_size;

    startTimer (kDebounceTimeMs);
}
