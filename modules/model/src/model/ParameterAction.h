#pragma once
#include "ParameterModel.h"

#include <lager/util.hpp>
#include <variant>

template <typename T>
struct UpdateParameterAction
{
    T value;
    T ParameterModel::*const member_ptr;
};

static auto UpdateRoomSize (float value)
{
    return UpdateParameterAction<float> {.value = value, .member_ptr = &ParameterModel::room_size};
}

static auto UpdateReverbTime (float value)
{
    return UpdateParameterAction<float> {.value = value,
                                         .member_ptr = &ParameterModel::reverb_time};
}

static auto UpdateDryWetMix (float value)
{
    return UpdateParameterAction<float> {.value = value,
                                         .member_ptr = &ParameterModel::dry_wet_mix};
}

using ParameterAction = std::variant<UpdateParameterAction<float>>;
ParameterModel UpdateParameter (ParameterModel model, ParameterAction action);
