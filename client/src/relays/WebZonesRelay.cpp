#include "WebZonesRelay.h"

#include "../controllers/WebZonesHelper.h"
#include "format/ZoneMetadataJson.h"
#include "preferences/Preferences.h"

WebZonesRelay::WebZonesRelay (juce::WebBrowserComponent & web_browser_component)
    : web_browser_component_ (web_browser_component)
{
}

juce::WebBrowserComponent::Options
WebZonesRelay::buildOptions (const juce::WebBrowserComponent::Options & initialOptions)
{
    return initialOptions
        .withNativeFunction ("get_cached_web_zone_native",
                             [&] (auto & var, auto complete)
                             {
                                 std::string zone_id;
                                 json::parse (var [0].toString ().toStdString ()).get_to (zone_id);

                                 WebZonesHelper web_zones_helper;
                                 json data = web_zones_helper.GetCachedWebZoneMetadata (zone_id);

                                 JUCE_ASSERT_MESSAGE_THREAD;
                                 complete ({data.dump ()});
                             })
        .withNativeFunction ("get_cached_web_zones_native",
                             [&] (auto & var, auto complete)
                             {
                                 WebZonesHelper web_zones_helper;
                                 json data = web_zones_helper.GetCachedWebZones ();

                                 JUCE_ASSERT_MESSAGE_THREAD;
                                 complete ({data.dump ()});
                             });
}