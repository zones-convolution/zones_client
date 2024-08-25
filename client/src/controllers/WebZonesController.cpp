#include "WebZonesController.h"

#include <algorithm>

static const std::filesystem::path kS3Host = "https://minio.zonesconvolution.com";
static const std::filesystem::path kImpulseResponsesBucket = "impulse-responses-processed";

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

static juce::URL
GetIrDownloadUrl (const std::string & zone_id, const std::string & ir_id, const std::string & file)
{
    auto ir_resource_url = kS3Host / kImpulseResponsesBucket / zone_id / ir_id / file;
    return {ir_resource_url.string ()};
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

bool WebZonesController::LoadWebZone (const IrSelection & ir_selection)
{
    auto zone_id = ir_selection.zone.zone_id;
    if (! zone_id)
        return false;

    auto & target_ir = ir_selection.ir;
    auto zone_resource_directory = GetZoneResourceDirectory (*zone_id);
    auto zone_metadata_path = zone_resource_directory / (*zone_id + ".json");

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
        return true;
    }

    auto position_map = *target_ir.position_map;

    std::vector<std::unique_ptr<juce::URL::DownloadTask>> download_tasks;

    // THIS NEEDS REFACTORING!!!! - DOWNLOADS
    if (position_map.centre.has_value ())
        download_tasks.push_back (DownloadZoneIr (
            *zone_id, *target_ir.ir_id, target_ir.relative_path, *position_map.centre));
    if (position_map.left.has_value ())
        download_tasks.push_back (DownloadZoneIr (
            *zone_id, *target_ir.ir_id, target_ir.relative_path, *position_map.left));
    if (position_map.right.has_value ())
        download_tasks.push_back (DownloadZoneIr (
            *zone_id, *target_ir.ir_id, target_ir.relative_path, *position_map.right));

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
        return false;

    irs.push_back (target_ir);

    auto zone_metadata = ir_selection.zone;
    zone_metadata.irs = irs;

    juce::File {zone_metadata_path.string ()}.create ();
    WriteZoneMetadata (zone_metadata_path, zone_metadata);

    return true;
}
