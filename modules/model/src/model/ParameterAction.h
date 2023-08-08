#pragma once
#include "ParameterModel.h"

#include <lager/util.hpp>
#include <variant>

template <typename Model, typename T>
struct UpdateParameterAction
{
    T value;
    T Model::*const member_ptr;
};

template <typename T>
using UpdateIrEngineParameterAction = UpdateParameterAction<IrEngineParameterModel, T>;

static auto UpdateRoomSize (float value)
{
    return UpdateIrEngineParameterAction<float> {.value = value,
                                                 .member_ptr = &IrEngineParameterModel::room_size};
}

static auto UpdateReverbTime (float value)
{
    return UpdateIrEngineParameterAction<float> {
        .value = value, .member_ptr = &IrEngineParameterModel::reverb_time};
}

using IrEngineParameterAction = std::variant<UpdateIrEngineParameterAction<float>>;
IrEngineParameterModel UpdateIrEngineParameter (IrEngineParameterModel model,
                                                IrEngineParameterAction action);

template <typename T>
using UpdateRealtimeParameterAction = UpdateParameterAction<RealtimeParameterModel, T>;

static auto UpdateDryWetMix (float value)
{
    return UpdateRealtimeParameterAction<float> {
        .value = value, .member_ptr = &RealtimeParameterModel::dry_wet_mix};
}

static auto UpdateInputGain (float value)
{
    return UpdateRealtimeParameterAction<float> {.value = value,
                                                 .member_ptr = &RealtimeParameterModel::input_gain};
}

static auto UpdateOutputGain (float value)
{
    return UpdateRealtimeParameterAction<float> {
        .value = value, .member_ptr = &RealtimeParameterModel::output_gain};
}

using RealtimeParameterAction = std::variant<UpdateRealtimeParameterAction<float>>;
RealtimeParameterModel UpdateRealtimeParameter (RealtimeParameterModel model,
                                                RealtimeParameterAction action);
