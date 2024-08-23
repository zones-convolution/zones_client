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

static void DownloadZoneIr (const std::string & zone_id,
                            const std::string & ir_id,
                            const std::string & relative_path,
                            const std::string & file)
{
    auto zone_resource_directory = GetZoneResourceDirectory (zone_id);
    auto ir_resource_path = zone_resource_directory / relative_path;
    auto ir_resource_url = kS3Host / kImpulseResponsesBucket / zone_id / ir_id;

    auto url = ir_resource_url / file;
    auto path = ir_resource_path / file;

    auto download_url = juce::URL {url.string ()};
    juce::File ir_file {path.string ()};
    ir_file.create ();

    auto download_task = download_url.downloadToFile (ir_file, juce::URL::DownloadTaskOptions {});
    while (! download_task->isFinished ())
    {
    }
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

    // THIS NEEDS REFACTORING!!!! - DOWNLOADS
    if (position_map.centre.has_value ())
        DownloadZoneIr (*zone_id, *target_ir.ir_id, target_ir.relative_path, "centre.wav");
    if (position_map.left.has_value ())
        DownloadZoneIr (*zone_id, *target_ir.ir_id, target_ir.relative_path, "left.wav");
    if (position_map.right.has_value ())
        DownloadZoneIr (*zone_id, *target_ir.ir_id, target_ir.relative_path, "right.wav");

    irs.push_back (target_ir);

    auto zone_metadata = ir_selection.zone;
    zone_metadata.irs = irs;

    juce::File {zone_metadata_path.string ()}.create ();
    WriteZoneMetadata (zone_metadata_path, zone_metadata);

    return true;
}
