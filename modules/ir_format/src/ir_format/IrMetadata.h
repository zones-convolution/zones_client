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

    /**
     * Runtime added absolute path to directory where specific IR is stored.
     */
    std::filesystem::path path_attribute;

    bool operator== (const IrMetadata & rhs) const
    {
        return std::tie (channel_format, position_map, name, description, path_attribute) ==
               std::tie (rhs.channel_format,
                         rhs.position_map,
                         rhs.name,
                         rhs.description,
                         rhs.path_attribute);
    }

    static juce::DynamicObject ToDynamic (const IrMetadata & ir_metadata);
    static IrMetadata FromDynamic (const juce::DynamicObject & ir_metadata_dynamic);
    static std::string ChannelFormatToString (ChannelFormat channel_format);

private:
    static juce::var PositionMapToVar (const PositionMap & position_map);
    static std::optional<PositionMap> PositionMapFromVar (const juce::var & position_map_var);

    static const juce::Identifier kNameIdentifier;
    static const juce::Identifier kDescriptionIdentifier;
    static const juce::Identifier kChannelFormatIdentifier;
    static const juce::Identifier kPositionMapIdentifier;
};