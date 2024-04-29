#pragma once

#include "ir_format/IrData.h"

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

struct IrMetadata
{
    std::optional<ChannelFormat> channel_format;
    std::optional<PositionMap> position_map;
    std::optional<std::string> title;
    std::optional<std::string> description;
};

struct ImageMetadata
{
    std::string image_id;
    std::string image_path;
};

struct ZoneMetadata
{
    std::string title;
    std::optional<std::string> description;
    std::vector<ImageMetadata> images;
    std::optional<std::string> cover_image_id;
    std::vector<IrMetadata> irs;

    /**
     * Runtime added absolute path to directory where specific IR is stored.
     */
    std::filesystem::path path_attribute;
};

void ReadZoneMetadata (const std::filesystem::path & absolute_file_path,
                       ZoneMetadata & zone_metadata);
