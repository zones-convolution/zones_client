#include "ParameterRelay.h"

ParameterRelay::ParameterRelay (juce::WebBrowserComponent & web_browser_component)
    : web_browser_component_ (web_browser_component)
{
}

juce::WebBrowserComponent::Options
ParameterRelay::buildOptions (const juce::WebBrowserComponent::Options & initialOptions)
{
    return initialOptions.withOptionsFrom (wet_dry_mix_relay_)
        .withOptionsFrom (resampler_relay_)
        .withOptionsFrom (room_size_relay_)
        .withOptionsFrom (input_gain_relay_)
        .withOptionsFrom (output_gain_relay_)
        .withOptionsFrom (treble_relay_)
        .withOptionsFrom (bass_relay_)
        .withOptionsFrom (reverb_time_relay_)
        .withOptionsFrom (predelay_relay_)
        .withOptionsFrom (trim_relay_);
}

ParameterAttachments::ParameterAttachments (ParameterRelay & parameter_relay,
                                            juce::AudioProcessorValueTreeState & parameter_tree)
    : wet_dry_mix_attachment_ (*parameter_tree.getParameter (ParameterTree::kDryWetMixParameterId),
                               parameter_relay.wet_dry_mix_relay_)
    , resampler_attachment_ (*parameter_tree.getParameter (ParameterTree::kResamplerParameterId),
                             parameter_relay.resampler_relay_)
    , room_size_attachment_ (*parameter_tree.getParameter (ParameterTree::kRoomSizeParameterId),
                             parameter_relay.room_size_relay_)
    , input_gain_attachment_ (*parameter_tree.getParameter (ParameterTree::kInputGainParameterId),
                              parameter_relay.input_gain_relay_)
    , output_gain_attachment_ (*parameter_tree.getParameter (ParameterTree::kOutputGainParameterId),
                               parameter_relay.output_gain_relay_)
    , treble_attachment_ (*parameter_tree.getParameter (ParameterTree::kTrebleParameterId),
                          parameter_relay.treble_relay_)
    , bass_attachment_ (*parameter_tree.getParameter (ParameterTree::kBassParameterId),
                        parameter_relay.bass_relay_)
    , reverb_time_attachment_ (*parameter_tree.getParameter (ParameterTree::kReverbTimeParameterId),
                               parameter_relay.reverb_time_relay_)
    , predelay_attachment_ (*parameter_tree.getParameter (ParameterTree::kPredelayParameterId),
                            parameter_relay.predelay_relay_)
    , trim_attachment_ (*parameter_tree.getParameter (ParameterTree::kTrimParameterId),
                        parameter_relay.trim_relay_)

{
}
