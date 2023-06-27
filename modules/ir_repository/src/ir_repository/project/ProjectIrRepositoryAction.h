#pragma once

#include "ProjectIrRepositoryModel.h"

#include <filesystem>
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

ProjectIrRepositoryModel Update (ProjectIrRepositoryModel model, ProjectIrRepositoryAction action)
{
    return std::visit (lager::visitor {[&] (AddProjectPathAction add_project_path_action)
                                       {
                                           model.project_paths = model.project_paths.push_back (
                                               add_project_path_action.project_path);
                                           return model;
                                       },
                                       [&] (RemoveProjectPathAction remove_project_path_action)
                                       {
                                           model.project_paths = model.project_paths.erase (
                                               remove_project_path_action.remove_at_index);
                                           return model;
                                       }},
                       action);
}