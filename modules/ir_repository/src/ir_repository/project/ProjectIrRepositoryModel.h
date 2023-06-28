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

enum class ImportingProjectIrState
{
    kSuccess,
    kFailure,
    kPending,
    kLoading
};

struct ProjectIrRepositoryModel
{
    immer::flex_vector<std::filesystem::path> project_paths;
    std::optional<ImportProjectIr> import_project_ir;
    ImportingProjectIrState importing_project_ir_state;
};
