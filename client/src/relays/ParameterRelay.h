#pragma once

#include "model/ParameterTree.h"

#include <juce_gui_extra/juce_gui_extra.h>

class ParameterRelay : public OptionsBuilder<juce::WebBrowserComponent::Options>
{
public:
    ParameterRelay (juce::WebBrowserComponent & web_browser_component,
                    juce::AudioProcessorValueTreeState & parameter_tree);
    ~ParameterRelay () override = default;

    juce::WebBrowserComponent::Options
    buildOptions (const juce::WebBrowserComponent::Options & initialOptions) override;

private:
    juce::WebBrowserComponent & web_browser_component_;

    juce::WebSliderRelay wet_dry_mix_relay_ {web_browser_component_,
                                             ParameterTree::kDryWetMixParameterId};
    juce::WebSliderRelay resampler_relay_ {web_browser_component_,
                                           ParameterTree::kResamplerParameterId};
    juce::WebSliderRelay room_size_relay_ {web_browser_component_,
                                           ParameterTree::kRoomSizeParameterId};

    juce::WebSliderParameterAttachment wet_dry_mix_attachment_;
    juce::WebSliderParameterAttachment resampler_attachment_;
    juce::WebSliderParameterAttachment room_size_attachment_;
};
