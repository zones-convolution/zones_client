#pragma once

#include "model/ParameterTree.h"

#include <juce_gui_extra/juce_gui_extra.h>

class ParameterAttachments;

class ParameterRelay : public OptionsBuilder<juce::WebBrowserComponent::Options>
{
public:
    friend class ParameterAttachments;

    explicit ParameterRelay (juce::WebBrowserComponent & web_browser_component);
    ~ParameterRelay () override = default;

    juce::WebBrowserComponent::Options
    buildOptions (const juce::WebBrowserComponent::Options & initialOptions) override;

private:
    juce::WebBrowserComponent & web_browser_component_;

    juce::WebSliderRelay wet_dry_mix_relay_ {ParameterTree::kDryWetMixParameterId};
    juce::WebSliderRelay resampler_relay_ {ParameterTree::kResamplerParameterId};
    juce::WebSliderRelay input_gain_relay_ {ParameterTree::kInputGainParameterId};
    juce::WebSliderRelay output_gain_relay_ {ParameterTree::kOutputGainParameterId};
    juce::WebSliderRelay treble_relay_ {ParameterTree::kTrebleParameterId};
    juce::WebSliderRelay bass_relay_ {ParameterTree::kBassParameterId};
    juce::WebSliderRelay reverb_time_relay_ {ParameterTree::kReverbTimeParameterId};
    juce::WebSliderRelay predelay_relay_ {ParameterTree::kPredelayParameterId};
    juce::WebSliderRelay trim_relay_ {ParameterTree::kTrimParameterId};
    juce::WebSliderRelay attack_relay_ {ParameterTree::kAttackParameterId};
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
    juce::WebSliderParameterAttachment input_gain_attachment_;
    juce::WebSliderParameterAttachment output_gain_attachment_;
    juce::WebSliderParameterAttachment treble_attachment_;
    juce::WebSliderParameterAttachment bass_attachment_;
    juce::WebSliderParameterAttachment reverb_time_attachment_;
    juce::WebSliderParameterAttachment predelay_attachment_;
    juce::WebSliderParameterAttachment trim_attachment_;
    juce::WebSliderParameterAttachment attack_attachment_;
};
