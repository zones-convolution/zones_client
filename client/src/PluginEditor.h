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
    explicit AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor &);
    ~AudioPluginAudioProcessorEditor () override = default;

private:
    static constexpr float m_preferredAspectRatio = 3.f / 4.f;
    static constexpr int m_windowMinimumWidth = 600;
    static constexpr int m_windowMaxWidth = 2000;
    static constexpr int m_workingSafeArea = 10;

    AudioPluginAudioProcessor & processorRef;

    zones::ApiRequestService m_apiRequestService {};

    zones::Sidebar m_sidebar;
    zones::Browser m_browser;

    zones::ZonesLookAndFeel m_zonesLookAndFeel;
    zones::SidebarLayout m_sidebarLayout;

    SpectrogramVisualiserComponent spectrogram_visualiser_;

    void resized () override;
    void paint (juce::Graphics & graphics) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};