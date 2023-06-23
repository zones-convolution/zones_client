#include "ir_repository/IrMetadata.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE ("ir_metadata reads and writes non-null attributes", "[IrMetadata]")
{
    static const std::string test_name = "test_name";
    static const std::string test_description = "test_description";
    IrMetadata ir_metadata {.name = test_name, .description = test_description};

    auto write_metadata_dynamic = IrMetadata::ToDynamic (ir_metadata);
    auto read_ir_metadata = IrMetadata::FromDynamic (write_metadata_dynamic);

    REQUIRE (read_ir_metadata.name == test_name);
    REQUIRE (read_ir_metadata.description == test_description);
}

TEST_CASE ("ir_metadata reads invalid metadata", "[IrMetadata]")
{
    juce::DynamicObject invalid_metadata_dynamic;
    invalid_metadata_dynamic.setProperty (juce::Identifier ("unknown_attribute"), -1);
    invalid_metadata_dynamic.setProperty (juce::Identifier ("name"), -0.1);
    invalid_metadata_dynamic.setProperty (juce::Identifier ("description"), -0.1);

    auto read_ir_metadata = IrMetadata::FromDynamic (invalid_metadata_dynamic);

    REQUIRE_FALSE (read_ir_metadata.name.has_value ());
    REQUIRE_FALSE (read_ir_metadata.description.has_value ());
}