#pragma once

#include "../controllers/LoadController.h"

#include <juce_gui_extra/juce_gui_extra.h>

class LoadRelay : public OptionsBuilder<juce::WebBrowserComponent::Options>
{
public:
    LoadRelay (juce::WebBrowserComponent & web_browser_component, LoadController & load_controller);
    ~LoadRelay () override = default;

    juce::WebBrowserComponent::Options
    buildOptions (const juce::WebBrowserComponent::Options & initialOptions) override;

private:
    rocket::scoped_connection_container connections_;
    juce::WebBrowserComponent & web_browser_component_;
    LoadController & load_controller_;

    JUCE_DECLARE_WEAK_REFERENCEABLE (LoadRelay)
};
