#pragma once

#include <filesystem>
#include <immer/flex_vector.hpp>
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
};
