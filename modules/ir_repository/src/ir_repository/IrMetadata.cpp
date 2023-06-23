#include "IrMetadata.h"

const juce::Identifier IrMetadata::kNameIdentifier = "name";
const juce::Identifier IrMetadata::kDescriptionIdentifier = "description";

juce::DynamicObject IrMetadata::ToDynamic (const IrMetadata & ir_metadata)
{
    juce::DynamicObject dynamic_object;

    if (ir_metadata.name.has_value ())
        dynamic_object.setProperty (kNameIdentifier, juce::var (ir_metadata.name.value ()));

    if (ir_metadata.description.has_value ())
        dynamic_object.setProperty (kDescriptionIdentifier,
                                    juce::var (ir_metadata.description.value ()));

    return dynamic_object;
}
IrMetadata IrMetadata::FromDynamic (const juce::DynamicObject & ir_metadata_dynamic)
{
    IrMetadata ir_metadata;
    const auto & name_var = ir_metadata_dynamic.getProperty (kNameIdentifier);
    const auto & description_var = ir_metadata_dynamic.getProperty (kDescriptionIdentifier);

    if (name_var.isString ())
        ir_metadata.name = name_var.toString ().toStdString ();

    if (description_var.isString ())
        ir_metadata.description = description_var.toString ().toStdString ();

    return ir_metadata;
}
