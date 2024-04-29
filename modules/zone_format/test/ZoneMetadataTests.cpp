#include "zone_format/ZoneMetadata.h"

#include <catch2/catch_test_macros.hpp>
#include <juce_core/juce_core.h>

static std::filesystem::path kTestDataDirectory = TEST_DATA_DIRECTORY;

TEST_CASE ("reads valid zone metadata", "[ZoneMetadata]")
{
    ZoneMetadata zone_metadata;
    ReadZoneMetadata (kTestDataDirectory / "valid_zone_metadata.json", zone_metadata);

    REQUIRE (zone_metadata.title == "title");
    REQUIRE (zone_metadata.description == "description");
    REQUIRE (zone_metadata.images.empty ());
    REQUIRE (zone_metadata.irs.empty ());
    REQUIRE (zone_metadata.cover_image_id == std::nullopt);
}

TEST_CASE ("writes zone metadata", "[ZoneMetadata]")
{
    auto metadata_file_path = kTestDataDirectory / "test_write_zone.json";

    ZoneMetadata write_zone_metadata;
    write_zone_metadata.title = "title";
    write_zone_metadata.description = "description";
    write_zone_metadata.cover_image_id = std::nullopt;
    write_zone_metadata.irs = {{
        .channel_format = ChannelFormat::kStereo,
        .position_map = PositionMap {.centre = "c", .left = "l", .right = "r"},
        .title = "title",
        .description = "description",
    }};
    write_zone_metadata.images = {{.image_id = "image_id", .image_path = "image_path"}};

    WriteZoneMetadata (metadata_file_path, write_zone_metadata);

    ZoneMetadata read_zone_metadata;
    ReadZoneMetadata (metadata_file_path, read_zone_metadata);

    REQUIRE (read_zone_metadata == write_zone_metadata);

    juce::File {metadata_file_path.string ()}.moveToTrash ();
}