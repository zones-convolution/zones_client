#pragma once

#include "audio_engine/player/PlayerController.h"

#include <juce_gui_extra/juce_gui_extra.h>
#include <rocket.hpp>

class PlayerRelay : public OptionsBuilder<juce::WebBrowserComponent::Options>
{
public:
    PlayerRelay (juce::WebBrowserComponent & web_browser_component,
                 PlayerController & player_controller);
    ~PlayerRelay () override = default;

    juce::WebBrowserComponent::Options
    buildOptions (const juce::WebBrowserComponent::Options & initialOptions) override;

private:
    rocket::scoped_connection_container connections_;
    juce::WebBrowserComponent & web_browser_component_;
    PlayerController & player_controller_;
};
