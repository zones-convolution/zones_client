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

    bool operator== (const IrMetadata & rhs) const
    {
        return std::tie (channel_format, position_map, title, description) ==
               std::tie (rhs.channel_format, rhs.position_map, rhs.title, rhs.description);
    }
};

struct ImageMetadata
{
    std::string image_id;
    std::string image_path;

    bool operator== (const ImageMetadata & rhs) const
    {
        return std::tie (image_id, image_path) == std::tie (rhs.image_id, rhs.image_path);
    }
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

    bool operator== (const ZoneMetadata & rhs) const
    {
        return std::tie (title, description, images, cover_image_id, irs, path_attribute) ==
               std::tie (rhs.title,
                         rhs.description,
                         rhs.images,
                         rhs.cover_image_id,
                         rhs.irs,
                         rhs.path_attribute);
    }
};

void ReadZoneMetadata (const std::filesystem::path & absolute_file_path,
                       ZoneMetadata & zone_metadata);

void WriteZoneMetadata (const std::filesystem::path & absolute_file_path,
                        const ZoneMetadata & zone_metadata);
