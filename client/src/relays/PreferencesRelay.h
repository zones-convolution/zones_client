#pragma once

#include "preferences/PreferencesController.h"

#include <juce_gui_extra/juce_gui_extra.h>
#include <rocket.hpp>

class PreferencesRelay : public OptionsBuilder<juce::WebBrowserComponent::Options>
{
public:
    PreferencesRelay (juce::WebBrowserComponent & web_browser_component,
                      PreferencesController & preferences_controller);
    ~PreferencesRelay () override = default;

    juce::WebBrowserComponent::Options
    buildOptions (const juce::WebBrowserComponent::Options & initialOptions) override;

private:
    rocket::scoped_connection_container connections_;
    juce::WebBrowserComponent & web_browser_component_;
    PreferencesController & preferences_controller_;

    JUCE_DECLARE_WEAK_REFERENCEABLE (PreferencesRelay)
};
