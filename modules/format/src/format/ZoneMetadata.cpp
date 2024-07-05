#include "ZoneMetadata.h"

#include "ZoneMetadataJson.h"

#include <fstream>

void ReadZoneMetadata (const std::filesystem::path & absolute_file_path,
                       ZoneMetadata & zone_metadata)
{
    std::ifstream stream (absolute_file_path);
    json::parse (stream).get_to (zone_metadata);
}

void WriteZoneMetadata (const std::filesystem::path & absolute_file_path,
                        const ZoneMetadata & zone_metadata)
{
    json data = zone_metadata.WithNullRuntimeAttributes ();
    std::ofstream stream (absolute_file_path);
    stream << std::setw (4) << data << std::endl;
}
