#pragma once

#include <immer/box.hpp>
#include <juce_core/juce_core.h>

struct IrEngineParameterModel
{
    float room_size = 1.f;
    float reverb_time = 1.f;

    bool operator== (const IrEngineParameterModel & parameter_model) const
    {
        return juce::approximatelyEqual (room_size, parameter_model.room_size) &&
               juce::approximatelyEqual (reverb_time, parameter_model.reverb_time);
    }
};

using BoxedIrEngineParameterModel = immer::box<IrEngineParameterModel>;

struct RealtimeParameterModel
{
    float dry_wet_mix = 1.f;

    bool operator== (const RealtimeParameterModel & parameter_model) const
    {
        return juce::approximatelyEqual (dry_wet_mix, parameter_model.dry_wet_mix);
    }
};

using BoxedRealtimeParameterModel = immer::box<RealtimeParameterModel>;
