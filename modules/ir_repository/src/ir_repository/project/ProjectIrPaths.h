#pragma once
#include "ProjectIrRepositoryModel.h"

#include <filesystem>
#include <lager/reader.hpp>
#include <optional>

class ProjectIrPaths
{
public:
    explicit ProjectIrPaths (lager::reader<ProjectIrRepositoryModel> reader);
    std::optional<std::filesystem::path> GetAvailableProjectPath ();

private:
    lager::reader<ProjectIrRepositoryModel> reader_;
};
