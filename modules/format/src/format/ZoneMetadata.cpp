#include "ZoneMetadata.h"

#include "JsonUtility.h"

#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

NLOHMANN_JSON_SERIALIZE_ENUM (ChannelFormat,
                              {
                                  {ChannelFormat::kMono, "mono"},
                                  {ChannelFormat::kStereo, "stereo"},
                                  {ChannelFormat::kFoa, "foa"},
                              })

static void from_json (const json & data, PositionMap & position_map)
{
    data.at ("centre").get_to (position_map.centre);
    data.at ("left").get_to (position_map.left);
    data.at ("right").get_to (position_map.right);
}

static void to_json (json & data, const PositionMap & position_map)
{
    data = json {{"centre", position_map.centre},
                 {"left", position_map.left},
                 {"right", position_map.right}};
}

static void from_json (const json & data, IrMetadata & ir_metadata)
{
    data.at ("channel_format").get_to (ir_metadata.channel_format);
    data.at ("position_map").get_to (ir_metadata.position_map);
    data.at ("title").get_to (ir_metadata.title);
    data.at ("description").get_to (ir_metadata.description);
    data.at ("relative_path").get_to (ir_metadata.relative_path);
}

static void to_json (json & data, const IrMetadata & ir_metadata)
{
    data = json {{"channel_format", ir_metadata.channel_format},
                 {"position_map", ir_metadata.position_map},
                 {"title", ir_metadata.title},
                 {"description", ir_metadata.description},
                 {"relative_path", ir_metadata.relative_path}};
}

static void from_json (const json & data, ImageMetadata & image_metadata)
{
    data.at ("image_id").get_to (image_metadata.image_id);
    data.at ("image_path").get_to (image_metadata.image_path);
}

static void to_json (json & data, const ImageMetadata & image_metadata)
{
    data = json {{"image_id", image_metadata.image_id}, {"image_path", image_metadata.image_path}};
}

static void from_json (const json & data, ZoneMetadata & zone_metadata)
{
    data.at ("title").get_to (zone_metadata.title);
    data.at ("description").get_to (zone_metadata.description);
    data.at ("images").get_to (zone_metadata.images);
    data.at ("cover_image_id").get_to (zone_metadata.cover_image_id);
    data.at ("irs").get_to (zone_metadata.irs);
}

static void to_json (json & data, const ZoneMetadata & zone_metadata)
{
    data = json {{"title", zone_metadata.title},
                 {"description", zone_metadata.description},
                 {"images", zone_metadata.images},
                 {"cover_image_id", zone_metadata.cover_image_id},
                 {"irs", zone_metadata.irs}};
}

void ReadZoneMetadata (const std::filesystem::path & absolute_file_path,
                       ZoneMetadata & zone_metadata)
{
    std::ifstream stream (absolute_file_path);
    json::parse (stream).get_to (zone_metadata);
}

void WriteZoneMetadata (const std::filesystem::path & absolute_file_path,
                        const ZoneMetadata & zone_metadata)
{
    json data = zone_metadata;
    std::ofstream stream (absolute_file_path);
    stream << std::setw (4) << data << std::endl;
}
