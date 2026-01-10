#pragma once

#include "format/IrData.h"
#include "format/ZoneMetadata.h"

#include <juce_gui_basics/juce_gui_basics.h>

class WebZonesHelper
{
public:
    enum class LoadWebZoneResult
    {
        kFailedToLoad,
        kLoadFromDisk,
        kLoadFromRemote
    };

    std::optional<ZoneMetadata> LoadWebZone (const IrSelection & ir_selection);
    std::optional<juce::File> LoadWebZoneImage (const std::string & zone_id,
                                                const std::string & image_id);

    [[nodiscard]] std::optional<ZoneMetadata>
    GetCachedWebZoneMetadata (std::string & zone_id) const;
    [[nodiscard]] std::vector<ZoneMetadata> GetCachedWebZones () const;

private:
    [[nodiscard]] std::optional<juce::File>
    GetCachedWebZoneImage (const std::string & zone_id, const std::string & image_id) const;
};
