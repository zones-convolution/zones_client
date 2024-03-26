#pragma once

#include "ProjectIrRepositoryModel.h"

#include <filesystem>
#include <optional>

bool IsPathValidDirectory (const std::filesystem::path & path);

std::optional<std::filesystem::path>
GetAvailableProjectPath (const ProjectIrRepositoryModel & model);