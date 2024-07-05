#pragma once

#include "model/ParameterTree.h"

#include <juce_gui_extra/juce_gui_extra.h>

class ParameterAttachments;

class ParameterRelay : public OptionsBuilder<juce::WebBrowserComponent::Options>
{
public:
    friend class ParameterAttachments;

    ParameterRelay (juce::WebBrowserComponent & web_browser_component);
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
};

/**
 * This is annoying that this is needed. It appears that the destruction order of this needs to come
 * before the web browser component. However relays should appear before the construction of the
 * browser.
 */
class ParameterAttachments
{
public:
    ParameterAttachments (ParameterRelay & parameter_relay,
                          juce::AudioProcessorValueTreeState & parameter_tree);

private:
    juce::WebSliderParameterAttachment wet_dry_mix_attachment_;
    juce::WebSliderParameterAttachment resampler_attachment_;
    juce::WebSliderParameterAttachment room_size_attachment_;
};
