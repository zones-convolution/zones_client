#pragma once

#include <immer/box.hpp>
#include <juce_core/juce_core.h>

struct ParameterModel
{
    float room_size = 1.f;
    float dry_wet_mix = 1.f;
    float reverb_time = 1.f;

    bool operator== (const ParameterModel & parameter_model) const
    {
        return juce::approximatelyEqual (room_size, parameter_model.room_size) &&
               juce::approximatelyEqual (dry_wet_mix, parameter_model.dry_wet_mix) &&
               juce::approximatelyEqual (reverb_time, parameter_model.reverb_time);
    }
};

using BoxedParameterModel = immer::box<ParameterModel>;