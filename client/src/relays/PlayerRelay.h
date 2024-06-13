#pragma once

#include "audio_engine/player/PlayerController.h"
#include "zones_generated.h"

#include <juce_gui_extra/juce_gui_extra.h>

class PlayerRelay : public OptionsBuilder<juce::WebBrowserComponent::Options>
{
public:
    PlayerRelay (juce::WebBrowserComponent & web_browser_component,
                 PlayerController & player_controller);

    juce::WebBrowserComponent::Options
    buildOptions (const juce::WebBrowserComponent::Options & initialOptions) override;

private:
    juce::WebBrowserComponent & web_browser_component_;
    PlayerController & player_controller_;
};
