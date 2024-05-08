#pragma once

#include "ImportComponent.h"
#include "format/ZoneMetadata.h"
#include "juce_gui_basics/juce_gui_basics.h"

class ImportController
{
public:
    ImportController (ImportComponent & import_component);

private:
    [[nodiscard]] static PositionMap
    PerformCopyPositionMap (const PositionMap & position_map,
                            const std::filesystem::path & zone_directory,
                            const std::filesystem::path & ir_directory_path);

    ImportComponent & import_component_;
};
