#include "ParameterRelay.h"

ParameterRelay::ParameterRelay (juce::WebBrowserComponent & web_browser_component,
                                juce::AudioProcessorValueTreeState & parameter_tree)
    : web_browser_component_ (web_browser_component)
    , wet_dry_mix_attachment_ (*parameter_tree.getParameter (ParameterTree::kDryWetMixParameterId),
                               wet_dry_mix_relay_)
    , resampler_attachment_ (*parameter_tree.getParameter (ParameterTree::kResamplerParameterId),
                             resampler_relay_)
    , room_size_attachment_ (*parameter_tree.getParameter (ParameterTree::kRoomSizeParameterId),
                             room_size_relay_)
{
}

juce::WebBrowserComponent::Options
ParameterRelay::buildOptions (const juce::WebBrowserComponent::Options & initialOptions)
{
    return initialOptions.withOptionsFrom (wet_dry_mix_relay_)
        .withOptionsFrom (resampler_relay_)
        .withOptionsFrom (room_size_relay_);
}
