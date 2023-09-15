#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

class ParameterTree
{
public:
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

    static const juce::Identifier kParameterTreeIdentifier;

    static const juce::String kDryWetMixParameterId;
    static const juce::String kInputGainParameterId;
    static const juce::String kOutputGainParameterId;

    static const juce::String kReverbTimeParameterId;
    static const juce::String kRoomSizeParameterId;

    static juce::AudioProcessorValueTreeState::ParameterLayout CreateParameterLayout ();
};