#include "UserZonesRelay.h"

#include "format/ZoneMetadataJson.h"
#include "preferences/Preferences.h"

UserZonesRelay::UserZonesRelay (juce::WebBrowserComponent & web_browser_component)
    : web_browser_component_ (web_browser_component)
{
}

std::vector<ZoneMetadata> UserZonesRelay::LoadUserZones () const
{
    Preferences preferences;
    preferences.Load ();

    std::vector<ZoneMetadata> user_zones;

    for (auto & search_path : preferences.user_paths)
    {
        auto search_directory = juce::File (search_path.string ());
        auto zone_paths = search_directory.findChildFiles (juce::File::TypesOfFileToFind::findFiles,
                                                           true,
                                                           "*.json",
                                                           juce::File::FollowSymlinks::no);

        for (const auto & zone_path : zone_paths)
        {
            try
            {
                std::filesystem::path absolute_zone_path =
                    zone_path.getFullPathName ().toStdString ();
                ZoneMetadata zone_metadata;
                ReadZoneMetadata (absolute_zone_path, zone_metadata);
                zone_metadata.path_attribute = absolute_zone_path.remove_filename ();
                user_zones.push_back (zone_metadata);
            }
            catch (...)
            {
            }
        }
    }

    return user_zones;
}

juce::WebBrowserComponent::Options
UserZonesRelay::buildOptions (const juce::WebBrowserComponent::Options & initialOptions)
{
    return initialOptions
        .withNativeFunction ("get_user_zones_native",
                             [&] (auto & var, auto complete)
                             {
                                 json data = LoadUserZones ();
                                 complete ({data.dump ()});
                             })
        .withNativeFunction ("add_user_zone_native",
                             [&] (auto & var, auto complete) {

                             })
        .withNativeFunction ("remove_user_zone_native", [&] (auto & var, auto complete) {})
        .withNativeFunction ("choose_ir_path_native",
                             [&] (auto & var, auto complete) {
                                 user_zones_controller_.GetIrPath ([complete] (const auto & path)
                                                                   { complete ({path}); });
                             });
}