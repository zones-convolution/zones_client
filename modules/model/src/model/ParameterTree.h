#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

class ParameterTree
{
public:
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

    juce::AudioParameterFloat * dry_wet_parameter;
    juce::AudioParameterFloat * input_gain_parameter;
    juce::AudioParameterFloat * out_gain_parameter;
    juce::AudioParameterFloat * bass_parameter;
    juce::AudioParameterFloat * treble_parameter;
    juce::AudioParameterFloat * reverb_time_parameter;
    juce::AudioParameterFloat * resampler_parameter;
    juce::AudioParameterFloat * pre_delay_parameter;
    juce::AudioParameterFloat * trim_parameter;
    juce::AudioParameterFloat * attack_parameter;

    static ParameterTree
    CreateParameterTree (juce::AudioProcessorValueTreeState::ParameterLayout & layout);

    static const juce::Identifier kParameterTreeIdentifier;

    static const juce::String kDryWetMixParameterId;
    static const juce::String kInputGainParameterId;
    static const juce::String kOutputGainParameterId;

    static const juce::String kBassParameterId;
    static const juce::String kTrebleParameterId;

    static const juce::String kReverbTimeParameterId;
    static const juce::String kResamplerParameterId;

    static const juce::String kPredelayParameterId;
    static const juce::String kTrimParameterId;
    static const juce::String kAttackParameterId;
};
