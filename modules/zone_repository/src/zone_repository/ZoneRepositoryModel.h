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
    std::vector<TargetFormat> valid_target_formats;
    std::optional<IrSelection> current_ir;
    IrLoadingState ir_loading_state {};
};

using CurrentIrReader = lager::reader<std::optional<IrSelection>>;
using ValidTargetFormatsReader = lager::reader<std::vector<TargetFormat>>;