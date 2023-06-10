#include "PluginEditor.h"

#include "PluginProcessor.h"

AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor & p)
    : AudioProcessorEditor (&p)
    , processorRef (p)
    , m_sidebarLayout (m_sidebar, m_browser)
{
    juce::ignoreUnused (processorRef);
    juce::LookAndFeel::setDefaultLookAndFeel (&m_zonesLookAndFeel);

    setResizable (true, true);
    setResizeLimits (m_windowMinimumWidth,
                     static_cast<int> (m_windowMinimumWidth * m_preferredAspectRatio),
                     m_windowMaxWidth,
                     static_cast<int> (m_windowMaxWidth * m_preferredAspectRatio));
    addAndMakeVisible (m_sidebarLayout);

    m_apiRequestService.beginRequest (
        zones::ApiRequest ()
            .withBaseUrl ("https://zones-convolution.vercel.app")
            .withMethod (zones::ApiRequest::HttpVerb::GET,
                         "/zones/edbc0ed5-9f07-46b1-9514-0a79418a74f6"),
        {
            .success = [] (zones::ApiRequestJob::Result result) { DBG (result.data.toString ()); },
            .fail = [] (zones::ApiRequestJob::Result result) { DBG (result.status); },
            .progress = [] (float progress) { DBG ("PROGRESS: " + juce::String (progress)); },
            .start = [] () { DBG ("START"); },
        });

    addAndMakeVisible(spectrogram_visualiser_);
}

void AudioPluginAudioProcessorEditor::resized ()
{
//    m_sidebarLayout.setBounds (
//        juce::BorderSize (m_workingSafeArea).subtractedFrom (getLocalBounds ()));

    spectrogram_visualiser_.setBounds (juce::BorderSize (m_workingSafeArea).subtractedFrom (getLocalBounds ()));
}

void AudioPluginAudioProcessorEditor::paint (juce::Graphics & graphics)
{
    graphics.fillAll (juce::Colour::fromRGB (23, 26, 29));
}