#include "ParameterTree.h"

const juce::Identifier ParameterTree::kParameterTreeIdentifier {"zones_parameter_tree"};

const juce::String ParameterTree::kDryWetMixParameterId {"dry_wet_mix_parameter"};
const juce::String ParameterTree::kInputGainParameterId {"input_gain_parameter"};
const juce::String ParameterTree::kOutputGainParameterId {"output_gain_parameter"};

const juce::String ParameterTree::kBassParameterId {"bass_parameter"};
const juce::String ParameterTree::kTrebleParameterId {"treble_parameter"};

const juce::String ParameterTree::kReverbTimeParameterId {"reverb_time_parameter"};
const juce::String ParameterTree::kRoomSizeParameterId {"room_size_parameter"};
const juce::String ParameterTree::kResamplerParameterId {"resampler_parameter"};

juce::AudioProcessorValueTreeState::ParameterLayout ParameterTree::CreateParameterLayout ()
{
    auto room_size_attributes = juce::AudioParameterFloatAttributes ().withAutomatable (false);
    auto reverb_time_attributes = juce::AudioParameterFloatAttributes ().withAutomatable (false);
    auto resampler_attributes = juce::AudioParameterFloatAttributes ().withAutomatable (false);

    return {
        std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID {kDryWetMixParameterId, 1},
            "Dry/Wet Mix",
            juce::NormalisableRange<float> (0.f, 1.f, 0.1f),
            0.6f,
            juce::AudioParameterFloatAttributes ().withLabel ("%")),
        std::make_unique<juce::AudioParameterFloat> (juce::ParameterID {kInputGainParameterId, 1},
                                                     "Input",
                                                     juce::NormalisableRange<float> (0.f, 2.f),
                                                     1.f),
        std::make_unique<juce::AudioParameterFloat> (juce::ParameterID {kOutputGainParameterId, 1},
                                                     "Output",
                                                     juce::NormalisableRange<float> (0.f, 2.f),
                                                     1.0),
        std::make_unique<juce::AudioParameterFloat> (juce::ParameterID {kBassParameterId, 1},
                                                     juce::translate (kBassParameterId),
                                                     juce::NormalisableRange<float> (0.f, 2.f),
                                                     1.f),
        std::make_unique<juce::AudioParameterFloat> (juce::ParameterID {kTrebleParameterId, 1},
                                                     juce::translate (kTrebleParameterId),
                                                     juce::NormalisableRange<float> (0.f, 2.f),
                                                     1.f),
        std::make_unique<juce::AudioParameterFloat> (juce::ParameterID {kRoomSizeParameterId, 1},
                                                     "Room Size",
                                                     juce::NormalisableRange<float> (0.f, 1.f),
                                                     1.0,
                                                     room_size_attributes),
        std::make_unique<juce::AudioParameterFloat> (juce::ParameterID {kReverbTimeParameterId, 1},
                                                     juce::translate (kReverbTimeParameterId),
                                                     juce::NormalisableRange<float> (0.f, 1.f),
                                                     1.0,
                                                     reverb_time_attributes),
        std::make_unique<juce::AudioParameterFloat> (juce::ParameterID {kResamplerParameterId, 1},
                                                     "Resample",
                                                     juce::NormalisableRange<float> (0.1f, 2.f),
                                                     1.0,
                                                     resampler_attributes)};
}
