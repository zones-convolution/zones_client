#pragma once

#include "../controllers/WebZonesController.h"
#include "format/ZoneMetadata.h"

#include <juce_gui_extra/juce_gui_extra.h>

class WebZonesRelay : public OptionsBuilder<juce::WebBrowserComponent::Options>
{
public:
    WebZonesRelay (juce::WebBrowserComponent & web_browser_component);
    ~WebZonesRelay () override = default;

    juce::WebBrowserComponent::Options
    buildOptions (const juce::WebBrowserComponent::Options & initialOptions) override;

private:
    juce::WebBrowserComponent & web_browser_component_;
    WebZonesController web_zones_controller_;

    JUCE_DECLARE_WEAK_REFERENCEABLE (WebZonesRelay)
};
