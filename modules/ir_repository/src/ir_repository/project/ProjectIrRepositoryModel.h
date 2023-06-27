#pragma once

#include <filesystem>
#include <immer/flex_vector.hpp>

struct ProjectIrRepositoryModel
{
    immer::flex_vector<std::filesystem::path> project_paths;
};