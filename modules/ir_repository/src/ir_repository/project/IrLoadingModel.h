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

//    immer::flex_vector<std::filesystem::path> project_paths;

/**
 * Project IRs will be removed in favour of UserIR's
 *
 * Can add multiple UserIR paths which are saved in global settings (IE in a json file in
 * ~/Library/zones/settings.json)
 *
 * Plugin state saves a relative path to the IR, and its identifier
 * When plugin recalls, first UserIR path is searched using file system to see if the identifier is
 * there If it is, load it If not search next UserIR path.
 *
 */