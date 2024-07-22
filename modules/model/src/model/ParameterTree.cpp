#include "ParameterTree.h"

#include "ParameterUtils.h"

const juce::Identifier ParameterTree::kParameterTreeIdentifier {"zones_parameter_tree"};

const juce::String ParameterTree::kDryWetMixParameterId {"dry_wet_mix_parameter"};
const juce::String ParameterTree::kInputGainParameterId {"input_gain_parameter"};
const juce::String ParameterTree::kOutputGainParameterId {"output_gain_parameter"};

const juce::String ParameterTree::kBassParameterId {"bass_parameter"};
const juce::String ParameterTree::kTrebleParameterId {"treble_parameter"};

const juce::String ParameterTree::kReverbTimeParameterId {"reverb_time_parameter"};
const juce::String ParameterTree::kResamplerParameterId {"resampler_parameter"};

const juce::String ParameterTree::kPredelayParameterId {"predelay_parameter"};
const juce::String ParameterTree::kTrimParameterId {"trim_parameter"};
const juce::String ParameterTree::kAttackParameterId {"attack_parameter"};

ParameterTree
ParameterTree::CreateParameterTree (juce::AudioProcessorValueTreeState::ParameterLayout & layout)
{
    return {
        .dry_wet_parameter = ParameterUtils::BindLayoutParameter (
            std::make_unique<juce::AudioParameterFloat> (
                juce::ParameterID {kDryWetMixParameterId, 1},
                "Dry/Wet Mix",
                juce::NormalisableRange<float> (0.f, 100.f),
                50.0f,
                juce::AudioParameterFloatAttributes ().withLabel ("%")),
            layout),
        .input_gain_parameter = ParameterUtils::BindLayoutParameter (
            std::make_unique<juce::AudioParameterFloat> (
                juce::ParameterID {kInputGainParameterId, 1},
                "Input",
                ParameterUtils::WithSkewedCentre (juce::NormalisableRange<float> (-100.0f, 10.f),
                                                  -6.f),
                0.f,
                juce::AudioParameterFloatAttributes ().withLabel ("dB")),
            layout),
        .out_gain_parameter = ParameterUtils::BindLayoutParameter (
            std::make_unique<juce::AudioParameterFloat> (
                juce::ParameterID {kOutputGainParameterId, 1},
                "Output",
                ParameterUtils::WithSkewedCentre (juce::NormalisableRange<float> (-100.0f, 10.f),
                                                  -6.f),
                0.0f,
                juce::AudioParameterFloatAttributes ().withLabel ("dB")),
            layout),
        .bass_parameter = ParameterUtils::BindLayoutParameter (
            std::make_unique<juce::AudioParameterFloat> (
                juce::ParameterID {kBassParameterId, 1},
                "Bass",
                juce::NormalisableRange<float> (-6.f, 6.f),
                0.f,
                juce::AudioParameterFloatAttributes ().withLabel ("dB")),
            layout),
        .treble_parameter = ParameterUtils::BindLayoutParameter (
            std::make_unique<juce::AudioParameterFloat> (
                juce::ParameterID {kTrebleParameterId, 1},
                "Treble",
                juce::NormalisableRange<float> (-6.f, 6.f),
                0.f,
                juce::AudioParameterFloatAttributes ().withLabel ("dB")),
            layout),
        .reverb_time_parameter = ParameterUtils::BindLayoutParameter (
            std::make_unique<juce::AudioParameterFloat> (
                juce::ParameterID {kReverbTimeParameterId, 1},
                "Reverb Time",
                juce::NormalisableRange<float> (0.f, 100.f),
                0.0f,
                juce::AudioParameterFloatAttributes ().withAutomatable (false).withLabel ("%")),
            layout),
        .resampler_parameter = ParameterUtils::BindLayoutParameter (
            std::make_unique<juce::AudioParameterFloat> (
                juce::ParameterID {kResamplerParameterId, 1},
                "Resample",
                juce::NormalisableRange<float> (20.0f, 180.f),
                100.0f,
                juce::AudioParameterFloatAttributes ().withAutomatable (false).withLabel ("%")),
            layout),
        .pre_delay_parameter = ParameterUtils::BindLayoutParameter (
            std::make_unique<juce::AudioParameterFloat> (
                juce::ParameterID {kPredelayParameterId, 1},
                "Predelay",
                juce::NormalisableRange<float> (0.0f, 500.0f),
                0.0f,
                juce::AudioParameterFloatAttributes ().withAutomatable (false).withLabel ("ms")),
            layout),
        .trim_parameter = ParameterUtils::BindLayoutParameter (
            std::make_unique<juce::AudioParameterFloat> (
                juce::ParameterID {kTrimParameterId, 1},
                "Trim",
                juce::NormalisableRange<float> (0.0f, 100.0f),
                0.0f,
                juce::AudioParameterFloatAttributes ().withAutomatable (false).withLabel ("%")),
            layout),
        .attack_parameter = ParameterUtils::BindLayoutParameter (
            std::make_unique<juce::AudioParameterFloat> (
                juce::ParameterID {kAttackParameterId, 1},
                "Attack",
                juce::NormalisableRange<float> (0.0f, 100.0f),
                0.0f,
                juce::AudioParameterFloatAttributes ().withAutomatable (false).withLabel ("%")),
            layout)};
}
