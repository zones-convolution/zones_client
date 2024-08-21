#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

enum class ZoneType
{
    kUser,
    kWeb
};

enum class ChannelFormat
{
    kMono,
    kStereo,
    kFoa,
    kQuadraphonic
};

struct PositionMap
{
    std::optional<std::string> centre;
    std::optional<std::string> left;
    std::optional<std::string> right;

    bool operator== (const PositionMap & rhs) const
    {
        return std::tie (centre, left, right) == std::tie (rhs.centre, rhs.left, rhs.right);
    }
};

struct IrMetadata
{
    std::optional<std::string> ir_id;
    std::optional<ChannelFormat> channel_format;
    std::optional<PositionMap> position_map;
    std::optional<std::string> title;
    std::optional<std::string> description;
    std::filesystem::path relative_path;

    bool operator== (const IrMetadata & rhs) const
    {
        return std::tie (ir_id, channel_format, position_map, title, description, relative_path) ==
               std::tie (ir_id,
                         rhs.channel_format,
                         rhs.position_map,
                         rhs.title,
                         rhs.description,
                         rhs.relative_path);
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
    ZoneType zone_type;
    std::string title;
    std::optional<std::string> zone_id;
    std::optional<std::string> description;
    std::vector<ImageMetadata> images;
    std::optional<std::string> cover_image_id;
    std::vector<IrMetadata> irs;

    /**
     * Runtime added absolute path to directory where specific IR is stored.
     */
    std::optional<std::filesystem::path> path_attribute;

    [[nodiscard]] ZoneMetadata WithNullRuntimeAttributes () const
    {
        auto metadata = *this;
        metadata.path_attribute = std::nullopt;
        return metadata;
    }

    bool operator== (const ZoneMetadata & rhs) const
    {
        return std::tie (zone_type,
                         title,
                         zone_id,
                         description,
                         images,
                         cover_image_id,
                         irs,
                         path_attribute) == std::tie (rhs.zone_type,
                                                      rhs.title,
                                                      rhs.zone_id,
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
