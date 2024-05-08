#include "ImportController.h"

ImportController::ImportController (ImportComponent & import_component)
    : import_component_ (import_component)
{
    import_component.OnSubmit = [&]
    {
        auto & import_zone = import_component_.import_zone_;
        auto & import_irs = import_component_.import_irs_;

        auto destination_path = std::filesystem::path (import_zone.GetUserPath ());

        auto zone_title = import_zone.GetTitle ();
        auto safe_zone_title =
            std::filesystem::path (juce::File::createLegalFileName (zone_title).toStdString ());

        auto zone_directory_path = destination_path / safe_zone_title;
        juce::File zone_directory {zone_directory_path.string ()};
        zone_directory.createDirectory ();

        ZoneMetadata zone_metadata;
        zone_metadata.title = zone_title;
        zone_metadata.description = import_zone.GetDescription ();

        for (auto & import_ir : import_irs)
        {
            IrMetadata ir_metadata;

            auto ir_title = import_ir->GetTitle ();
            auto safe_ir_title =
                std::filesystem::path (juce::File::createLegalFileName (ir_title).toStdString ());

            ir_metadata.title = ir_title;
            ir_metadata.description = import_ir->GetDescription ();
            ir_metadata.channel_format = import_ir->GetChannelFormat ();
            ir_metadata.position_map =
                PerformCopyPositionMap (import_ir->speaker_position_component_.GetPositionMap (),
                                        zone_directory_path,
                                        "impulse_responses" / safe_ir_title);

            zone_metadata.irs.push_back (ir_metadata);
        }

        WriteZoneMetadata (zone_directory_path / safe_zone_title.replace_extension (".json"),
                           zone_metadata);
    };
}

PositionMap ImportController::PerformCopyPositionMap (const PositionMap & position_map,
                                                      const std::filesystem::path & zone_directory,
                                                      const std::filesystem::path & ir_path)
{
    auto ir_directory_path = zone_directory / ir_path;
    juce::File ir_directory {ir_directory_path.string ()};
    ir_directory.createDirectory ();

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
    translated_map.centre = copy_position (position_map.centre, ir_path / "centre");
    translated_map.left = copy_position (position_map.left, ir_path / "left");
    translated_map.right = copy_position (position_map.right, ir_path / "right");

    return translated_map;
}
