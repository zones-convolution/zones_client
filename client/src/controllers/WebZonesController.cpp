#include "WebZonesController.h"

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

static ZoneMetadata CreatePartialMetadataFromSelection (const IrSelection & ir_selection)
{
    auto partial_metadata = ir_selection.zone;
    partial_metadata.irs = {ir_selection.ir};
    return partial_metadata;
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

    auto & ir = ir_selection.ir;
    auto zone_resource_directory = GetZoneResourceDirectory (*zone_id);
    auto zone_metadata_path = zone_resource_directory / (*zone_id + ".json");
    auto ir_resource_path = zone_resource_directory / ir.relative_path;

    try
    {
        ZoneMetadata zone_metadata;
        ReadZoneMetadata (zone_metadata_path, zone_metadata);
    }
    catch (...)
    {
        if (ir.position_map->centre.has_value ())
            DownloadZoneIr (*zone_id, *ir.ir_id, ir.relative_path, "centre.wav");
        if (ir.position_map->left.has_value ())
            DownloadZoneIr (*zone_id, *ir.ir_id, ir.relative_path, "left.wav");
        if (ir.position_map->right.has_value ())
            DownloadZoneIr (*zone_id, *ir.ir_id, ir.relative_path, "right.wav");

        auto partial_metadata = CreatePartialMetadataFromSelection (ir_selection);
        auto zones_data_directory = GetZonesDataDirectory ();
        juce::File {zone_metadata_path.string ()}.create ();
        WriteZoneMetadata (zone_metadata_path, partial_metadata);
    }
}
