#pragma once

#include "../controllers/UserZonesController.h"
#include "format/ZoneMetadata.h"

#include <juce_gui_extra/juce_gui_extra.h>

class UserZonesRelay : public OptionsBuilder<juce::WebBrowserComponent::Options>
{
public:
    UserZonesRelay (juce::WebBrowserComponent & web_browser_component);
    ~UserZonesRelay () override = default;

    juce::WebBrowserComponent::Options
    buildOptions (const juce::WebBrowserComponent::Options & initialOptions) override;

private:
    std::vector<ZoneMetadata> LoadUserZones () const;
    juce::WebBrowserComponent & web_browser_component_;
    UserZonesController user_zones_controller_;

    JUCE_DECLARE_WEAK_REFERENCEABLE (UserZonesRelay)
};
