#pragma once

#include "IrData.h"

#include <juce_core/juce_core.h>
#include <optional>

struct IrMetadata
{
public:
    std::optional<ChannelFormat> channel_format;
    std::optional<PositionMap> position_map;
    std::optional<std::string> name;
    std::optional<std::string> description;

    static juce::DynamicObject ToDynamic (const IrMetadata & ir_metadata);
    static IrMetadata FromDynamic (const juce::DynamicObject & ir_metadata_dynamic);

private:
    static juce::var PositionMapToVar (const PositionMap & position_map);
    static std::optional<PositionMap> PositionMapFromVar (const juce::var & position_map_var);

    static const juce::Identifier kNameIdentifier;
    static const juce::Identifier kDescriptionIdentifier;
    static const juce::Identifier kChannelFormatIdentifier;
    static const juce::Identifier kPositionMapIdentifier;
};