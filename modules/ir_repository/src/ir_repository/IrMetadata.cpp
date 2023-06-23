#include "IrMetadata.h"

const juce::Identifier IrMetadata::kNameIdentifier = "name";
const juce::Identifier IrMetadata::kDescriptionIdentifier = "description";

juce::DynamicObject IrMetadata::ToDynamic ()
{
    juce::DynamicObject dynamic_object;

    if (name.has_value ())
        dynamic_object.setProperty (kNameIdentifier, juce::var (name.value ()));

    if (description.has_value ())
        dynamic_object.setProperty (kDescriptionIdentifier, juce::var (description.value ()));

    return dynamic_object;
}

IrMetadata::IrMetadata (const juce::DynamicObject dynamic_object)
{
    const auto & name_var = dynamic_object.getProperty (kNameIdentifier);
    const auto & description_var = dynamic_object.getProperty (kDescriptionIdentifier);

    if (name_var.isString ())
        name = name_var.toString ().toStdString ();

    if (description_var.isString ())
        description = description_var.toString ().toStdString ();
}
