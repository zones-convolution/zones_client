#pragma once

#include "ImportComponent.h"
#include "format/ZoneMetadata.h"
#include "juce_gui_basics/juce_gui_basics.h"
#include "layout/tabs/TabsAction.h"

class ImportController
{
public:
    ImportController (ImportComponent & import_component, lager::context<TabsAction> tabs_context);

private:
    [[nodiscard]] static PositionMap
    PerformCopyPositionMap (const PositionMap & position_map,
                            const std::filesystem::path & zone_directory,
                            const std::filesystem::path & ir_directory_path);

    ImportComponent & import_component_;
    lager::context<TabsAction> tabs_context_;
};
