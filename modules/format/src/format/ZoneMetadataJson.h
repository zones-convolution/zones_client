#pragma once

#include "JsonUtility.h"
#include "ZoneMetadata.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

NLOHMANN_JSON_SERIALIZE_ENUM (ChannelFormat,
                              {{ChannelFormat::kMono, "mono"},
                               {ChannelFormat::kStereo, "stereo"},
                               {ChannelFormat::kFoa, "foa"},
                               {ChannelFormat::kQuadraphonic, "quadraphonic"}})

static void from_json (const json & data, PositionMap & position_map)
{
    OptionalFromJson (data, "centre", position_map.centre);
    OptionalFromJson (data, "left", position_map.left);
    OptionalFromJson (data, "right", position_map.right);
}

static void to_json (json & data, const PositionMap & position_map)
{
    OptionalToJson (data, "centre", position_map.centre);
    OptionalToJson (data, "left", position_map.left);
    OptionalToJson (data, "right", position_map.right);
}

static void from_json (const json & data, IrMetadata & ir_metadata)
{
    OptionalFromJson (data, "channelFormat", ir_metadata.channel_format);
    OptionalFromJson (data, "positionMap", ir_metadata.position_map);
    OptionalFromJson (data, "title", ir_metadata.title);
    OptionalFromJson (data, "description", ir_metadata.description);
    data.at ("relativePath").get_to (ir_metadata.relative_path);
}

static void to_json (json & data, const IrMetadata & ir_metadata)
{
    OptionalToJson (data, "channelFormat", ir_metadata.channel_format);
    OptionalToJson (data, "positionMap", ir_metadata.position_map);
    OptionalToJson (data, "title", ir_metadata.title);
    OptionalToJson (data, "description", ir_metadata.description);
    data = json {{"relativePath", ir_metadata.relative_path}};
}

static void from_json (const json & data, ImageMetadata & image_metadata)
{
    data.at ("imageId").get_to (image_metadata.image_id);
    data.at ("imagePath").get_to (image_metadata.image_path);
}

static void to_json (json & data, const ImageMetadata & image_metadata)
{
    data = json {{"imageId", image_metadata.image_id}, {"imagePath", image_metadata.image_path}};
}

static void from_json (const json & data, ZoneMetadata & zone_metadata)
{
    data.at ("title").get_to (zone_metadata.title);
    OptionalFromJson (data, "description", zone_metadata.description);
    data.at ("images").get_to (zone_metadata.images);
    OptionalFromJson (data, "coverImageId", zone_metadata.cover_image_id);
    data.at ("irs").get_to (zone_metadata.irs);
}

static void to_json (json & data, const ZoneMetadata & zone_metadata)
{
    OptionalToJson (data, "description", zone_metadata.description);
    OptionalToJson (data, "coverImageId", zone_metadata.cover_image_id);
    data = json {{"title", zone_metadata.title},
                 {"images", zone_metadata.images},
                 {"irs", zone_metadata.irs}};
}