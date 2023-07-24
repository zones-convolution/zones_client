#pragma once

#include <immer/box.hpp>
#include <juce_core/juce_core.h>

struct ParameterModel
{
    float room_size;
    float dry_wet_mix;

    bool operator== (const ParameterModel & parameter_model) const
    {
        return juce::approximatelyEqual (room_size, parameter_model.room_size) &&
               juce::approximatelyEqual (dry_wet_mix, parameter_model.dry_wet_mix);
    }
};

using BoxedParameterModel = immer::box<ParameterModel>;