#include "format/IrMetadata.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE ("ir_metadata reads and writes non-null attributes", "[IrMetadata]")
{
    static const std::string test_name = "test_name";
    static const std::string test_description = "test_description";
    static const auto channel_format = ChannelFormat::kFoa;
    static const PositionMap position_map {.centre = "centre", .left = "left", .right = "right"};

    IrMetadata ir_metadata {.channel_format = channel_format,
                            .position_map = position_map,
                            .name = test_name,
                            .description = test_description};

    auto write_metadata_dynamic = IrMetadata::ToDynamic (ir_metadata);
    auto read_ir_metadata = IrMetadata::FromDynamic (write_metadata_dynamic);

    REQUIRE (read_ir_metadata.name == test_name);
    REQUIRE (read_ir_metadata.description == test_description);
    REQUIRE (read_ir_metadata.channel_format == channel_format);

    REQUIRE (read_ir_metadata.position_map->left == position_map.left);
    REQUIRE (read_ir_metadata.position_map->right == position_map.right);
    REQUIRE (read_ir_metadata.position_map->centre == position_map.centre);
}

TEST_CASE ("ir_metadata reads empty metadata", "[IrMetadata]")
{
    juce::DynamicObject empty_metadata_dynamic;
    auto read_ir_metadata = IrMetadata::FromDynamic (empty_metadata_dynamic);

    REQUIRE_FALSE (read_ir_metadata.channel_format.has_value ());
    REQUIRE_FALSE (read_ir_metadata.position_map.has_value ());
    REQUIRE_FALSE (read_ir_metadata.name.has_value ());
    REQUIRE_FALSE (read_ir_metadata.description.has_value ());
}

TEST_CASE ("ir_metadata reads invalid metadata", "[IrMetadata]")
{
    juce::DynamicObject invalid_metadata_dynamic;

    invalid_metadata_dynamic.setProperty (juce::Identifier ("unknown_attribute"), -1);
    invalid_metadata_dynamic.setProperty (juce::Identifier ("channel_format"), -0.1);
    invalid_metadata_dynamic.setProperty (juce::Identifier ("position_map"), -0.1);
    invalid_metadata_dynamic.setProperty (juce::Identifier ("name"), -0.1);
    invalid_metadata_dynamic.setProperty (juce::Identifier ("description"), -0.1);

    auto read_ir_metadata = IrMetadata::FromDynamic (invalid_metadata_dynamic);
    REQUIRE_FALSE (read_ir_metadata.channel_format.has_value ());
    REQUIRE_FALSE (read_ir_metadata.position_map.has_value ());
    REQUIRE_FALSE (read_ir_metadata.name.has_value ());
    REQUIRE_FALSE (read_ir_metadata.description.has_value ());
}