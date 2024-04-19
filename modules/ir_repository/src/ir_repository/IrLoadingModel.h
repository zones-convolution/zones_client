#pragma once

#include <filesystem>
#include <immer/flex_vector.hpp>
#include <lager/lenses/attr.hpp>
#include <lager/reader.hpp>
#include <optional>

enum class IrLoadingState
{
    kSuccess,
    kFailure,
    kPending,
    kLoading
};

struct IrLoadingModel
{
    std::optional<std::filesystem::path> ir_path;
    IrLoadingState ir_loading_state;
};