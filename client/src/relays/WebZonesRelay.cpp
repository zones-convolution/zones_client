#include "WebZonesRelay.h"

#include "../controllers/WebZonesHelper.h"
#include "preferences/Preferences.h"

WebZonesRelay::WebZonesRelay (juce::WebBrowserComponent & web_browser_component,
                              LoadController & load_controller)
    : web_browser_component_ (web_browser_component)
{
    connections_ += {load_controller.OnZoneMetadataUpdated.connect (
        [&] (const auto & zone_metadata) { EmitCachedWebZoneUpdatedEvent (zone_metadata); })};
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

void WebZonesRelay::EmitCachedWebZoneUpdatedEvent (const ZoneMetadata & cached_web_zone)
{
    json data = cached_web_zone;
    JUCE_ASSERT_MESSAGE_THREAD;
    web_browser_component_.emitEventIfBrowserIsVisible ("on_cached_web_zone_updated",
                                                        {data.dump ()});
}

std::optional<juce::WebBrowserComponent::Resource>
WebZonesRelay::GetWebZoneImageResource (const std::string & zone_id,
                                        const std::string & image_id) const
{
    juce::WebBrowserComponent::Resource resource;
    resource.mimeType = "image/jpeg";

    WebZonesHelper web_zones_helper;
    auto image_file_opt = web_zones_helper.GetCachedWebZoneImage (zone_id, image_id);

    if (! image_file_opt.has_value ())
        return std::nullopt;

    const auto & image_file = *image_file_opt;
    if (! image_file.existsAsFile ())
        return std::nullopt;

    auto stream = image_file.createInputStream ();
    if (! stream->openedOk ())
        return std::nullopt;

    auto file_size = static_cast<int> (stream->getTotalLength ());
    resource.data.resize (file_size);
    stream->read (resource.data.data (), file_size);

    return resource;
}