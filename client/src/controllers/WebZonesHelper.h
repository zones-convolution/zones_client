#pragma once

#include "format/IrData.h"
#include "format/ZoneMetadata.h"

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

    [[nodiscard]] std::optional<ZoneMetadata>
    GetCachedWebZoneMetadata (std::string & zone_id) const;
    [[nodiscard]] std::vector<ZoneMetadata> GetCachedWebZones () const;
};
