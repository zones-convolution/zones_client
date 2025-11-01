#pragma once

#include "format/IrData.h"
#include "format/ZoneMetadata.h"

class WebZonesHelper
{
public:
    bool LoadWebZone (const IrSelection & ir_selection);
    [[nodiscard]] std::optional<ZoneMetadata>
    GetCachedWebZoneMetadata (std::string & zone_id) const;
    [[nodiscard]] std::vector<ZoneMetadata> GetCachedWebZones () const;
};
