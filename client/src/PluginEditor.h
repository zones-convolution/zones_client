#pragma once

#include "PluginProcessor.h"
#include "browser/Browser.h"
#include "layout/SidebarLayout.h"
#include "sidebar/Sidebar.h"
#include "spectrogram_visualiser/SpectrogramVisualiserComponent.h"
#include "zones_api/ApiRequestService.h"
#include "zones_look_and_feel/ZonesLookAndFeel.h"

#include <juce_gui_basics/juce_gui_basics.h>

class AudioPluginAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor & processor);
    ~AudioPluginAudioProcessorEditor () override = default;

private:
    static constexpr float kPreferredAspectRatio = 3.f / 4.f;
    static constexpr int kWindowMinimumWidth = 600;
    static constexpr int kWindowMaxWidth = 2000;
    static constexpr int kWorkingSafeArea = 10;

    AudioPluginAudioProcessor & audio_processor_;

    zones::ApiRequestService api_request_service_ {};

    zones::Sidebar sidebar_;
    zones::Browser browser_;

    zones::ZonesLookAndFeel zones_look_and_feel_;
    zones::SidebarLayout sidebar_layout_;

    SpectrogramVisualiserComponent spectrogram_visualiser_;

    void resized () override;
    void paint (juce::Graphics & graphics) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};