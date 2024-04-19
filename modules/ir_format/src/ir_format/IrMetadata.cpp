#include "IrMetadata.h"

const juce::Identifier IrMetadata::kNameIdentifier = "name";
const juce::Identifier IrMetadata::kDescriptionIdentifier = "description";
const juce::Identifier IrMetadata::kChannelFormatIdentifier = "channel_format";
const juce::Identifier IrMetadata::kPositionMapIdentifier = "position_map";

std::string IrMetadata::ChannelFormatToString (ChannelFormat channel_format)
{
    switch (channel_format)
    {
        case ChannelFormat::kMono:
            return "mono";
        case ChannelFormat::kStereo:
            return "stereo";
        case ChannelFormat::kFoa:
            return "foa";
    }
}

static std::unordered_map<std::string, ChannelFormat> kChannelFormatMap = {
    {"mono", ChannelFormat::kMono},
    {"stereo", ChannelFormat::kStereo},
    {"foa", ChannelFormat::kFoa}};

std::optional<ChannelFormat> StringToChannelFormat (std::string channel_format)
{
    if (kChannelFormatMap.contains (channel_format))
        return kChannelFormatMap [channel_format];
    else
        return std::nullopt;
}

juce::var IrMetadata::PositionMapToVar (const PositionMap & position_map)
{
    juce::DynamicObject * dynamic_object = new juce::DynamicObject ();

    if (position_map.centre.has_value ())
        dynamic_object->setProperty ("centre", juce::var (*position_map.centre));
    if (position_map.left.has_value ())
        dynamic_object->setProperty ("left", juce::var (*position_map.left));
    if (position_map.right.has_value ())
        dynamic_object->setProperty ("right", juce::var (*position_map.right));

    return {dynamic_object};
}

std::optional<PositionMap> IrMetadata::PositionMapFromVar (const juce::var & position_map_var)
{
    auto dynamic = position_map_var.getDynamicObject ();
    if (dynamic == nullptr)
        return std::nullopt;

    PositionMap position_map;
    const auto & centre_var = dynamic->getProperty ("centre");
    if (centre_var.isString ())
        position_map.centre = centre_var.toString ().toStdString ();

    const auto & left_var = dynamic->getProperty ("left");
    if (left_var.isString ())
        position_map.left = left_var.toString ().toStdString ();

    const auto & right_var = dynamic->getProperty ("right");
    if (right_var.isString ())
        position_map.right = right_var.toString ().toStdString ();
    return position_map;
}

juce::DynamicObject IrMetadata::ToDynamic (const IrMetadata & ir_metadata)
{
    juce::DynamicObject dynamic_object;

    if (ir_metadata.channel_format.has_value ())
        dynamic_object.setProperty (
            kChannelFormatIdentifier,
            juce::var (ChannelFormatToString (*ir_metadata.channel_format)));

    if (ir_metadata.position_map.has_value ())
        dynamic_object.setProperty (kPositionMapIdentifier,
                                    PositionMapToVar (*ir_metadata.position_map));

    if (ir_metadata.name.has_value ())
        dynamic_object.setProperty (kNameIdentifier, juce::var (*ir_metadata.name));

    if (ir_metadata.description.has_value ())
        dynamic_object.setProperty (kDescriptionIdentifier, juce::var (*ir_metadata.description));

    return dynamic_object;
}

IrMetadata IrMetadata::FromDynamic (const juce::DynamicObject & ir_metadata_dynamic)
{
    IrMetadata ir_metadata;

    const auto & channel_format_var = ir_metadata_dynamic.getProperty (kChannelFormatIdentifier);
    if (channel_format_var.isString ())
        ir_metadata.channel_format =
            StringToChannelFormat (channel_format_var.toString ().toStdString ());

    const auto & position_map_var = ir_metadata_dynamic.getProperty (kPositionMapIdentifier);
    ir_metadata.position_map = PositionMapFromVar (position_map_var);

    const auto & name_var = ir_metadata_dynamic.getProperty (kNameIdentifier);
    if (name_var.isString ())
        ir_metadata.name = name_var.toString ().toStdString ();

    const auto & description_var = ir_metadata_dynamic.getProperty (kDescriptionIdentifier);
    if (description_var.isString ())
        ir_metadata.description = description_var.toString ().toStdString ();

    return ir_metadata;
}
