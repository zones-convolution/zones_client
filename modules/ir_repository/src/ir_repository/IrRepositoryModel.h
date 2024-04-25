#pragma once

#include "ir_format/IrMetadata.h"

#include <immer/flex_vector.hpp>
#include <lager/reader.hpp>

enum class IrLoadingState
{
    kSuccess,
    kFailure,
    kPending,
    kLoading
};

struct IrRepositoryModel
{
    bool user_irs_loading = false;
    immer::flex_vector<IrMetadata> user_irs;

    std::optional<IrMetadata> current_ir_metadata;
    IrLoadingState ir_loading_state;
};

using CurrentIrReader = lager::reader<std::optional<IrMetadata>>;