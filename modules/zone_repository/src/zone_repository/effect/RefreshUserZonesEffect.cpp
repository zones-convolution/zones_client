#include "RefreshUserZonesEffect.h"

#include "format/ZoneMetadata.h"
#include "preferences/Preferences.h"

void RefreshUserZonesEffect (const RefreshUserZonesAction & action,
                             const ZoneRepositoryContext & context)
{
    context.loop ().async (
        [action, context]
        {
            Preferences preferences;
            preferences.Load ();

            RefreshUserZonesResultAction result;

            for (auto & search_path : preferences.user_paths)
            {
                auto search_directory = juce::File (search_path.string ());
                auto zone_paths =
                    search_directory.findChildFiles (juce::File::TypesOfFileToFind::findFiles,
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
                        result.user_zones = result.user_zones.push_back (zone_metadata);
                    }
                    catch (...)
                    {
                    }
                }
            }

            context.dispatch (result);
        });
}
