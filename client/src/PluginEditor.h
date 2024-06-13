#pragma once

#include "PluginProcessor.h"
#include "ProcessorContainer.h"
#include "preferences/PreferencesController.h"
#include "relays/PlayerRelay.h"
#include "relays/PreferencesRelay.h"

#include <juce_gui_extra/juce_gui_extra.h>
#include <lager/event_loop/manual.hpp>
#include <lager/store.hpp>

class AudioPluginAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor & processor,
                                              ProcessorContainer & processor_container);
    ~AudioPluginAudioProcessorEditor () override = default;

private:
    struct SinglePageBrowser : juce::WebBrowserComponent
    {
        using WebBrowserComponent::WebBrowserComponent;
        bool pageAboutToLoad (const juce::String & newURL) override;
    };

    static constexpr float kPreferredAspectRatio = 3.f / 4.f;
    static constexpr int kWindowMinimumWidth = 600;
    static constexpr int kWindowMaxWidth = 2000;
    static const juce::String kLocalDevServerAddress;

    void resized () override;
    std::optional<juce::WebBrowserComponent::Resource> GetResource (const juce::String & url);

    AudioPluginAudioProcessor & processor_;
    ProcessorContainer & processor_container_;
    PreferencesController preferences_controller_ {processor_container_.store_};

    juce::WebSliderRelay wet_dry_mix_relay_ {web_browser_component_,
                                             ParameterTree::kDryWetMixParameterId};
    PlayerRelay player_relay_;
    PreferencesRelay preferences_relay_;

    juce::File asset_directory_;
    const juce::WebBrowserComponent::Options kBaseWebOptions =
        juce::WebBrowserComponent::Options {}
            .withBackend (juce::WebBrowserComponent::Options::Backend::webview2)
            .withWinWebView2Options (
                juce::WebBrowserComponent::Options::WinWebView2 {}.withUserDataFolder (
                    juce::File::getSpecialLocation (
                        juce::File::SpecialLocationType::tempDirectory)))
            .withNativeIntegrationEnabled ()
            .withResourceProvider ([this] (const auto & url) { return GetResource (url); },
                                   juce::URL {kLocalDevServerAddress}.getOrigin ());

    SinglePageBrowser web_browser_component_;

    juce::WebSliderParameterAttachment wet_dry_mix_attachment_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
