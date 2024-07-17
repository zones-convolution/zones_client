#pragma once
#include <juce_audio_processors/juce_audio_processors.h>

struct ParameterUtils
{
    template <typename T>
    static T * BindLayoutParameter (std::unique_ptr<T> parameter,
                                    juce::AudioProcessorValueTreeState::ParameterLayout & layout)
    {
        auto p = parameter.get ();
        layout.add (std::move (parameter));
        return p;
    }

    template <typename T>
    static T * BindGroupParameter (std::unique_ptr<T> parameter,
                                   juce::AudioProcessorParameterGroup & group)
    {
        auto p = parameter.get ();
        group.addChild (std::move (parameter));
        return p;
    }

    static juce::NormalisableRange<float> WithSkewedCentre (juce::NormalisableRange<float> range,
                                                            float centre)
    {
        range.setSkewForCentre (centre);
        return range;
    }
};