#include "UserZonesController.h"

const juce::String UserZonesController::kChoosePickerDialogTitle = "Pick Audio File";

void UserZonesController::GetIrPath (
    std::function<void (const std::string & path)> add_ir_path_callback)
{
    file_picker_ = std::make_unique<juce::FileChooser> (
        kChoosePickerDialogTitle, juce::File (), "*.wav,*.aiff,*.mp3");
    auto picker_flags =
        juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;
    file_picker_->launchAsync (picker_flags,
                               [add_ir_path_callback] (const juce::FileChooser & chooser)
                               {
                                   auto path = chooser.getResult ();
                                   if (path.exists ())
                                       add_ir_path_callback (
                                           path.getFullPathName ().toStdString ());
                               });
}

bool UserZonesController::IsImportMetadataValid (ImportMetadata import_metadata)
{
    if (import_metadata.path.empty ())
        return false;

    auto & metadata = import_metadata.metadata;
    if (metadata.title.empty ())
        return false;
    if (metadata.irs.empty ())
        return false;

    for (auto & ir : metadata.irs)
    {
        if (! ir.title.has_value () || ir.title->empty ())
            return false;
    }
    return true;
}

bool UserZonesController::Import (ImportMetadata import_metadata)
{
    if (! IsImportMetadataValid (import_metadata))
        return false;

    try
    {
        auto & metadata = import_metadata.metadata;
        auto destination_path = std::filesystem::path (import_metadata.path);

        auto zone_title = metadata.title;
        auto safe_zone_title =
            std::filesystem::path (juce::File::createLegalFileName (zone_title).toStdString ());

        auto zone_directory_path = destination_path / safe_zone_title;
        juce::File zone_directory {zone_directory_path.string ()};
        zone_directory.createDirectory ();

        for (auto & import_ir : import_metadata.metadata.irs)
        {
            auto safe_ir_title = std::filesystem::path (
                juce::File::createLegalFileName (import_ir.title.value ()).toStdString ());
            auto relative_path = "impulse_responses" / safe_ir_title;
            import_ir.position_map = PerformCopyPositionMap (
                import_ir.position_map.value (), zone_directory_path, relative_path);
            import_ir.relative_path = relative_path;
        }
        WriteZoneMetadata (zone_directory_path / safe_zone_title.replace_extension (".json"),
                           metadata);
    }
    catch (...)
    {
        return false;
    }

    return true;
}

PositionMap
UserZonesController::PerformCopyPositionMap (const PositionMap & position_map,
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
    translated_map.centre = copy_position (position_map.centre, "centre");
    translated_map.left = copy_position (position_map.left, "left");
    translated_map.right = copy_position (position_map.right, "right");

    return translated_map;
}
