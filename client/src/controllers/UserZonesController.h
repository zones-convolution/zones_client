#pragma once
#include "format/ZoneMetadata.h"
#include "format/ZoneMetadataJson.h"

#include <juce_gui_basics/juce_gui_basics.h>

struct ImportMetadata
{
    std::string path;
    ZoneMetadata metadata;
};

class UserZonesController
{
public:
    UserZonesController () = default;
    void GetIrPath (std::function<void (const std::string & path)> add_ir_path_callback);
    bool Import (ImportMetadata import_metadata);

private:
    bool IsImportMetadataValid (ImportMetadata import_metadata);
    PositionMap PerformCopyPositionMap (const PositionMap & position_map,
                                        const std::filesystem::path & zone_directory,
                                        const std::filesystem::path & ir_path);

    static const juce::String kChoosePickerDialogTitle;
    std::unique_ptr<juce::FileChooser> file_picker_;
};

static void from_json (const json & data, ImportMetadata & import_metadata)
{
    data.at ("path").get_to (import_metadata.path);
    data.at ("metadata").get_to (import_metadata.metadata);
}

static void to_json (json & data, const ImportMetadata & import_metadata)
{
    data = json {{"path", import_metadata.path}, {"metadata", import_metadata.metadata}};
}