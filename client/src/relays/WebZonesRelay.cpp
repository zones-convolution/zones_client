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

static void EncodeImageToJPEG (const juce::Image & image, std::vector<std::byte> & image_data)
{
    juce::MemoryOutputStream output_stream;
    juce::JPEGImageFormat image_format;

    image_format.writeImageToStream (image, output_stream);
    auto raw_data = output_stream.getData ();
    auto data_size = output_stream.getDataSize ();
    image_data.clear ();
    image_data.resize (data_size);
    std::memcpy (image_data.data (), raw_data, data_size);
}

std::optional<juce::WebBrowserComponent::Resource>
WebZonesRelay::GetWebZoneImageResource (const std::string & zone_id,
                                        const std::string & image_id) const
{
    juce::WebBrowserComponent::Resource resource;
    resource.mimeType = "image/jpeg";

    WebZonesHelper web_zones_helper;
    auto image_load_result = web_zones_helper.LoadWebZoneImage (zone_id, image_id);
    if (image_load_result.has_value ())
    {
        EncodeImageToJPEG (*image_load_result, resource.data);
        return resource;
    }
    else
    {
        return std::nullopt;
    }
}
