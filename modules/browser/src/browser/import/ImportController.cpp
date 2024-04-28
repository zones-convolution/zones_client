#include "ImportController.h"

ImportController::ImportController (ImportComponent & import_component)
    : import_component_ (import_component)
{
    import_component.OnSubmit = [&]
    {
        auto destination_path = std::filesystem::path (import_component_.GetUserPath ());

        auto ir_name = import_component_.GetIrName ();
        auto safe_ir_name =
            std::filesystem::path (juce::File::createLegalFileName (ir_name).toStdString ());

        auto ir_directory_path = destination_path / safe_ir_name;
        juce::File ir_directory {ir_directory_path.string ()};

        ir_directory.createDirectory ();

        IrMetadata ir_metadata;

        ir_metadata.name = import_component_.GetIrName ();
        ir_metadata.description = import_component_.GetIrDescription ();
        ir_metadata.channel_format = import_component_.GetChannelFormat ();
        ir_metadata.position_map = PerformCopyPositionMap (
            import_component_.speaker_position_component_.GetPositionMap (), ir_directory_path);

        IrWriter ir_writer;

        auto absolute_metadata_path =
            ir_directory_path / IrDataFormat::GetMetadataFileNameForIdentifier (safe_ir_name);
        ir_writer.WriteIrMetadata (absolute_metadata_path, ir_metadata);
    };
}

PositionMap
ImportController::PerformCopyPositionMap (const PositionMap & position_map,
                                          const std::filesystem::path & ir_directory_path)
{
    auto copy_position =
        [&] (std::optional<std::string> position,
             const std::string & destination_file_name) -> std::optional<std::string>
    {
        if (position.has_value ())
        {
            juce::File file_to_copy {*position};
            if (file_to_copy.existsAsFile ())
            {
                auto extension = file_to_copy.getFileExtension ();

                auto destination_file_stem = destination_file_name + extension.toStdString ();
                auto destination_file_path = ir_directory_path / destination_file_stem;

                auto destination_file = juce::File (destination_file_path.string ());
                file_to_copy.copyFileTo (destination_file);

                return destination_file_stem;
            }
        }

        return std::nullopt;
    };

    PositionMap translated_map;
    translated_map.centre = copy_position (position_map.centre, "centre");
    translated_map.left = copy_position (position_map.left, "left");
    translated_map.right = copy_position (position_map.right, "right");

    return translated_map;
}
