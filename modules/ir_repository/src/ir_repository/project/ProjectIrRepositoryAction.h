#pragma once

#include "ProjectIrRepositoryModel.h"

#include <filesystem>
#include <lager/effect.hpp>
#include <lager/util.hpp>
#include <variant>

struct AddProjectPathAction
{
    std::filesystem::path project_path;
};

struct RemoveProjectPathAction
{
    size_t remove_at_index;
};

using ProjectIrRepositoryAction = std::variant<AddProjectPathAction, RemoveProjectPathAction>;

using ProjectIrRepositoryEffect =
    lager::effect<ProjectIrRepositoryAction, lager::deps<std::string &>>;

using ProjectIrRepositoryResult = std::pair<ProjectIrRepositoryModel, ProjectIrRepositoryEffect>;

ProjectIrRepositoryResult Update (ProjectIrRepositoryModel model, ProjectIrRepositoryAction action);
