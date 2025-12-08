#pragma once

#include "../controllers/LoadController.h"
#include "format/ZoneMetadataJson.h"

#include <juce_gui_extra/juce_gui_extra.h>
#include <rocket.hpp>

class WebZonesRelay : public OptionsBuilder<juce::WebBrowserComponent::Options>
{
public:
    WebZonesRelay (juce::WebBrowserComponent & web_browser_component,
                   LoadController & load_controller);
    ~WebZonesRelay () override = default;

    juce::WebBrowserComponent::Options
    buildOptions (const juce::WebBrowserComponent::Options & initialOptions) override;

private:
    rocket::scoped_connection_container connections_;

    void EmitCachedWebZoneUpdatedEvent (const ZoneMetadata & cached_web_zone);

    juce::WebBrowserComponent & web_browser_component_;
    JUCE_DECLARE_WEAK_REFERENCEABLE (WebZonesRelay)
};
