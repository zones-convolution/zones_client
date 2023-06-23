#pragma once

#include <juce_core/juce_core.h>
#include <optional>

struct IrMetadata
{
public:
    std::optional<std::string> name;
    std::optional<std::string> description;

    static juce::DynamicObject ToDynamic (const IrMetadata & ir_metadata);
    static IrMetadata FromDynamic (const juce::DynamicObject & ir_metadata_dynamic);

private:
    static const juce::Identifier kNameIdentifier;
    static const juce::Identifier kDescriptionIdentifier;
};
