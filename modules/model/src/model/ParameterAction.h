#pragma once
#include "ParameterModel.h"

#include <lager/util.hpp>
#include <variant>

struct SetRoomSizeAction
{
    float room_size;
};

struct SetDryWetMixAction
{
    float dry_wet_mix;
};

struct SetReverbTimeAction
{
    float reverb_time;
};

using ParameterAction = std::variant<SetRoomSizeAction, SetDryWetMixAction, SetReverbTimeAction>;

ParameterModel UpdateParameter (ParameterModel model, ParameterAction action);
