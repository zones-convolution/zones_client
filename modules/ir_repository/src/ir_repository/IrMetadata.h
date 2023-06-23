#pragma once

#include <juce_core/juce_core.h>
#include <optional>

struct IrMetadata
{
public:
    IrMetadata (const juce::DynamicObject dynamic_object);

    std::optional<std::string> name;
    std::optional<std::string> description;

    juce::DynamicObject ToDynamic ();

private:
    static const juce::Identifier kNameIdentifier;
    static const juce::Identifier kDescriptionIdentifier;
};
