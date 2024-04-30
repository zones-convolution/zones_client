#pragma once

#include "format/IrData.h"
#include "format/ZoneMetadata.h"

#include <immer/flex_vector.hpp>
#include <lager/reader.hpp>

enum class IrLoadingState
{
    kSuccess,
    kFailure,
    kPending,
    kLoading
};

struct ZoneRepositoryModel
{
    bool user_zones_loading = false;
    immer::flex_vector<ZoneMetadata> user_zones;
    std::optional<IrSelection> current_ir;
    IrLoadingState ir_loading_state {};
};

using UserZonesReader = lager::reader<immer::flex_vector<ZoneMetadata>>;
using CurrentIrReader = lager::reader<std::optional<IrSelection>>;