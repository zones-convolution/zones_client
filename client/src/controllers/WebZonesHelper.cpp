#include "WebZonesHelper.h"

#include <algorithm>

static const std::filesystem::path kS3Host = "https://minio.zonesconvolution.com";
static const std::filesystem::path kImpulseResponsesBucket = "impulse-responses-processed";
static const std::filesystem::path kImagesBucket = "images-processed";

static std::filesystem::path GetZonesDataDirectory ()
{
    return juce::File::getSpecialLocation (
               juce::File::SpecialLocationType::userApplicationDataDirectory)
        .getChildFile ("zones")
        .getChildFile ("zones")
        .getFullPathName ()
        .toStdString ();
}

static std::filesystem::path GetZoneResourceDirectory (const std::string & zone_id)
{
    return GetZonesDataDirectory () / zone_id;
}

static std::filesystem::path GetZoneMetadataPath (const std::string & zone_id)
{
    auto zone_resource_directory = GetZoneResourceDirectory (zone_id);
    return zone_resource_directory / (zone_id + ".json");
}

static std::filesystem::path GetZoneImagesDirectory (const std::string & zone_id)
{
    return GetZoneResourceDirectory (zone_id) / "images";
}

static std::filesystem::path GetCachedZoneImagePath (const std::string & zone_id,
                                                     const std::string & image_id)
{
    return GetZoneImagesDirectory (zone_id) / (image_id + ".jpeg");
}
static juce::URL
GetIrDownloadUrl (const std::string & zone_id, const std::string & ir_id, const std::string & file)
{
    auto ir_resource_url = kS3Host / kImpulseResponsesBucket / zone_id / ir_id / file;
    return {ir_resource_url.string ()};
}

static juce::URL GetImageDownloadUrl (const std::string & zone_id, const std::string & image_id)
{
    auto image_resource_url = kS3Host / kImagesBucket / zone_id / (image_id + ".jpeg");
    return {image_resource_url.string ()};
}

static juce::File
GetIrFile (const std::string & zone_id, const std::string & relative_path, const std::string & file)
{
    auto zone_resource_directory = GetZoneResourceDirectory (zone_id);
    auto ir_resource_path = zone_resource_directory / relative_path;
    auto path = ir_resource_path / file;
    return {path.string ()};
}

static std::unique_ptr<juce::URL::DownloadTask> DownloadZoneIr (const std::string & zone_id,
                                                                const std::string & ir_id,
                                                                const std::string & relative_path,
                                                                const std::string & file)
{
    auto download_url = GetIrDownloadUrl (zone_id, ir_id, file);
    auto ir_file = GetIrFile (zone_id, relative_path, file);
    ir_file.create ();
    return download_url.downloadToFile (ir_file, juce::URL::DownloadTaskOptions {});
}

static std::unique_ptr<juce::URL::DownloadTask> DownloadZoneImage (const std::string & zone_id,
                                                                   const std::string & image_id)
{
    auto download_url = GetImageDownloadUrl (zone_id, image_id);
    auto image_path = GetCachedZoneImagePath (zone_id, image_id);
    juce::File image_file {image_path.string ()};
    image_file.create ();
    return download_url.downloadToFile (image_file, juce::URL::DownloadTaskOptions {});
}

std::optional<ZoneMetadata> WebZonesHelper::GetCachedWebZoneMetadata (std::string & zone_id) const
{
    auto zone_metadata_path = GetZoneMetadataPath (zone_id);
    try
    {
        ZoneMetadata zone_metadata;
        ReadZoneMetadata (zone_metadata_path, zone_metadata);
        return zone_metadata;
    }
    catch (...)
    {
        return std::nullopt;
    }
}

std::vector<ZoneMetadata> WebZonesHelper::GetCachedWebZones () const
{
    std::vector<ZoneMetadata> cached_zones;

    auto search_directory = juce::File (GetZonesDataDirectory ().string ());

    auto zone_paths = search_directory.findChildFiles (
        juce::File::TypesOfFileToFind::findFiles, true, "*.json", juce::File::FollowSymlinks::no);

    for (const auto & zone_path : zone_paths)
    {
        try
        {
            std::filesystem::path absolute_zone_path = zone_path.getFullPathName ().toStdString ();
            ZoneMetadata zone_metadata;
            ReadZoneMetadata (absolute_zone_path, zone_metadata);
            //            zone_metadata.path_attribute = absolute_zone_path.remove_filename (); ->
            //            We might need this :DD ????
            cached_zones.push_back (zone_metadata);
        }
        catch (...)
        {
        }
    }

    return cached_zones;
}

std::optional<ZoneMetadata> WebZonesHelper::LoadWebZone (const IrSelection & ir_selection)
{
    auto zone_id = ir_selection.zone.zone_id;
    if (! zone_id)
        return std::nullopt;

    auto & target_ir = ir_selection.ir;
    auto zone_metadata_path = GetZoneMetadataPath (*zone_id);

    std::vector<IrMetadata> irs;

    try
    {
        ZoneMetadata zone_metadata;
        ReadZoneMetadata (zone_metadata_path, zone_metadata);
        irs = zone_metadata.irs;
    }
    catch (...)
    {
    }

    if (std::find (irs.begin (), irs.end (), target_ir) != irs.end ())
    {
        // check audio actually exists
        return ir_selection.zone;
    }

    auto position_map = *target_ir.position_map;

    std::vector<std::unique_ptr<juce::URL::DownloadTask>> download_tasks;

    if (position_map.centre.has_value ())
        download_tasks.push_back (DownloadZoneIr (
            *zone_id, *target_ir.ir_id, target_ir.relative_path.string (), *position_map.centre));
    if (position_map.left.has_value ())
        download_tasks.push_back (DownloadZoneIr (
            *zone_id, *target_ir.ir_id, target_ir.relative_path.string (), *position_map.left));
    if (position_map.right.has_value ())
        download_tasks.push_back (DownloadZoneIr (
            *zone_id, *target_ir.ir_id, target_ir.relative_path.string (), *position_map.right));

    auto download_complete = [&]
    {
        return std::all_of (download_tasks.begin (),
                            download_tasks.end (),
                            [] (const auto & download_task)
                            { return download_task->isFinished (); });
    };

    static constexpr auto kDownloadTimeout = 30000;
    auto timeout =
        juce::Time::getCurrentTime () + juce::RelativeTime::milliseconds (kDownloadTimeout);
    auto timed_out = [&] { return juce::Time::getCurrentTime () > timeout; };

    static constexpr auto kDownloadRefreshInterval = 200;
    while (! download_complete () && ! timed_out ())
        juce::Thread::sleep (kDownloadRefreshInterval);

    if (! download_complete ())
        return std::nullopt;

    irs.push_back (target_ir);

    auto zone_metadata = ir_selection.zone;
    zone_metadata.irs = irs;

    juce::File {zone_metadata_path.string ()}.create ();
    WriteZoneMetadata (zone_metadata_path, zone_metadata);

    return zone_metadata;
}

std::optional<juce::File> WebZonesHelper::LoadWebZoneImage (const std::string & zone_id,
                                                            const std::string & image_id)
{
    auto cached_image = GetCachedWebZoneImage (zone_id, image_id);
    if (cached_image.has_value ())
        return cached_image;

    auto image_download_task = DownloadZoneImage (zone_id, image_id);

    static constexpr auto kDownloadTimeout = 10000;
    auto timeout =
        juce::Time::getCurrentTime () + juce::RelativeTime::milliseconds (kDownloadTimeout);
    auto timed_out = [&] { return juce::Time::getCurrentTime () > timeout; };

    static constexpr auto kDownloadRefreshInterval = 200;
    while (! image_download_task->isFinished () && ! timed_out ())
        juce::Thread::sleep (kDownloadRefreshInterval);

    if (! image_download_task->isFinished ())
        return std::nullopt;

    return GetCachedWebZoneImage (zone_id, image_id);
}

std::optional<juce::File> WebZonesHelper::GetCachedWebZoneImage (const std::string & zone_id,
                                                                 const std::string & image_id) const
{
    auto image_path = GetCachedZoneImagePath (zone_id, image_id);
    if (! std::filesystem::exists (image_path))
        return std::nullopt;
    return juce::File (image_path.string ());
}