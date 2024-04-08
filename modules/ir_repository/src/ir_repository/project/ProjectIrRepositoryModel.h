#pragma once

#include <filesystem>
#include <immer/flex_vector.hpp>
#include <lager/lenses/attr.hpp>
#include <lager/reader.hpp>
#include <optional>

struct ImportProjectIr
{
    std::filesystem::path ir_path;
    std::string name;
    std::string description;
};

enum class ProjectIrLoadingState
{
    kSuccess,
    kFailure,
    kPending,
    kLoading
};

using CurrentProjectIrOptional = std::optional<std::string>;

struct ProjectIrRepositoryModel
{
    immer::flex_vector<std::filesystem::path> project_paths;

    ProjectIrLoadingState importing_project_ir_state;

    CurrentProjectIrOptional current_project_ir;
    ProjectIrLoadingState current_project_ir_state;

    static auto ProjectPathsReader (lager::reader<ProjectIrRepositoryModel> project_ir_reader)
    {
        return project_ir_reader.zoom (
            lager::lenses::attr (&ProjectIrRepositoryModel::project_paths));
    }

    static auto
    ImportingProjectIrStateReader (lager::reader<ProjectIrRepositoryModel> project_ir_reader)
    {
        return project_ir_reader.zoom (
            lager::lenses::attr (&ProjectIrRepositoryModel::importing_project_ir_state));
    }
};

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