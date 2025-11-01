#pragma once

#include "PluginProcessor.h"
#include "ProcessorContainer.h"
#include "preferences/PreferencesController.h"
#include "relays/EngineRelay.h"
#include "relays/LoadRelay.h"
#include "relays/MeteringRelay.h"
#include "relays/ParameterRelay.h"
#include "relays/PlayerRelay.h"
#include "relays/PreferencesRelay.h"
#include "relays/ResizeRelay.h"
#include "relays/UserZonesRelay.h"
#include "relays/VisualiserRelay.h"
#include "relays/WebZonesRelay.h"

#include <juce_gui_extra/juce_gui_extra.h>

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
    static constexpr int kWindowMinimumWidth = 750;
    static constexpr int kWindowMaxWidth = 2000;
    static const juce::String kLocalDevServerAddress;

    void resized () override;
    std::optional<juce::WebBrowserComponent::Resource> GetResource (const juce::String & url);

    AudioPluginAudioProcessor & processor_;
    ProcessorContainer & processor_container_;

    ParameterRelay parameter_relay_;
    EngineRelay engine_relay_;
    PlayerRelay player_relay_;
    PreferencesRelay preferences_relay_;
    UserZonesRelay user_zones_relay_;
    LoadRelay load_relay_;
    MeteringRelay metering_relay_;
    ResizeRelay resize_relay_;
    VisualiserRelay visualiser_relay_;
    WebZonesRelay web_zones_relay_;

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

    ParameterAttachments parameter_attachments_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
