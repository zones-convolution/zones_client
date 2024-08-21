#include "WebZonesRelay.h"

#include "format/ZoneMetadataJson.h"
#include "preferences/Preferences.h"

WebZonesRelay::WebZonesRelay (juce::WebBrowserComponent & web_browser_component)
    : web_browser_component_ (web_browser_component)
{
}

juce::WebBrowserComponent::Options
WebZonesRelay::buildOptions (const juce::WebBrowserComponent::Options & initialOptions)
{
    return initialOptions;
}