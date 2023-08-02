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

using ImportProjectIrOptional = std::optional<ImportProjectIr>;
using CurrentProjectIrOptional = std::optional<std::string>;

struct ProjectIrRepositoryModel
{
    immer::flex_vector<std::filesystem::path> project_paths;
    ImportProjectIrOptional import_project_ir;
    ProjectIrLoadingState importing_project_ir_state;

    CurrentProjectIrOptional current_project_ir;
    ProjectIrLoadingState current_project_ir_state;

    static auto ProjectPathsReader (lager::reader<ProjectIrRepositoryModel> project_ir_reader)
    {
        return project_ir_reader.zoom (
            lager::lenses::attr (&ProjectIrRepositoryModel::project_paths));
    }

    static auto CurrentProjectIrReader (lager::reader<ProjectIrRepositoryModel> project_ir_reader)
    {
        return project_ir_reader.zoom (
            lager::lenses::attr (&ProjectIrRepositoryModel::current_project_ir));
    }

    static auto
    ImportingProjectIrStateReader (lager::reader<ProjectIrRepositoryModel> project_ir_reader)
    {
        return project_ir_reader.zoom (
            lager::lenses::attr (&ProjectIrRepositoryModel::importing_project_ir_state));
    }

    static auto ImportProjectIrReader (lager::reader<ProjectIrRepositoryModel> project_ir_reader)
    {
        return project_ir_reader.zoom (
            lager::lenses::attr (&ProjectIrRepositoryModel::import_project_ir));
    }
};