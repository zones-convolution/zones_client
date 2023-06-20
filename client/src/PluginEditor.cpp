#include "PluginEditor.h"

#include "PluginProcessor.h"

AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (
    AudioPluginAudioProcessor & processor)
    : AudioProcessorEditor (&processor)
    , processor_ (processor)
    , sidebar_layout_ (sidebar_, browser_)
{
    juce::LookAndFeel::setDefaultLookAndFeel (&zones_look_and_feel_);

    setResizable (true, true);
    setResizeLimits (kWindowMinimumWidth,
                     static_cast<int> (kWindowMinimumWidth * kPreferredAspectRatio),
                     kWindowMaxWidth,
                     static_cast<int> (kWindowMaxWidth * kPreferredAspectRatio));
    addAndMakeVisible (sidebar_layout_);

    api_request_service_.beginRequest (
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

    addAndMakeVisible (spectrogram_visualiser_);
}

void AudioPluginAudioProcessorEditor::resized ()
{
    spectrogram_visualiser_.setBounds (
        juce::BorderSize (kWorkingSafeArea).subtractedFrom (getLocalBounds ()));
}

void AudioPluginAudioProcessorEditor::paint (juce::Graphics & graphics)
{
    graphics.fillAll (juce::Colour::fromRGB (23, 26, 29));
}