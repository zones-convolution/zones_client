#pragma once

#include "ir_format/IrMetadata.h"

#include <immer/flex_vector.hpp>

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
    
    std::optional<std::filesystem::path> ir_path;
    IrLoadingState ir_loading_state;
};