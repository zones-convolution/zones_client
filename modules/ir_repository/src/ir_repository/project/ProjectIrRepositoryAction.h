#pragma once

#include "ProjectIrRepositoryModel.h"
#include "ir_engine/IrController.h"

#include <filesystem>
#include <lager/effect.hpp>
#include <variant>

struct AddProjectPathAction
{
    std::filesystem::path project_path;
};

struct RemoveProjectPathAction
{
    size_t remove_at_index;
};

struct ImportProjectIrAction
{
    ImportProjectIr import_project_ir;
};

struct ImportProjectIrLoadingAction
{
};

struct ImportProjectIrSuccessAction
{
};

struct ImportProjectIrFailureAction
{
};

struct LoadProjectIrAction
{
    std::string ir_identifier;
};

struct LoadProjectIrLoadingAction
{
};

struct LoadProjectIrSuccessAction
{
    std::string ir_identifier;
};

struct LoadProjectIrFailureAction
{
};

using ProjectIrRepositoryAction = std::variant<AddProjectPathAction,
                                               RemoveProjectPathAction,
                                               ImportProjectIrAction,
                                               ImportProjectIrLoadingAction,
                                               ImportProjectIrSuccessAction,
                                               ImportProjectIrFailureAction,
                                               LoadProjectIrAction,
                                               LoadProjectIrLoadingAction,
                                               LoadProjectIrSuccessAction,
                                               LoadProjectIrFailureAction>;

using ProjectIrRepositoryResult =
    lager::result<ProjectIrRepositoryModel, ProjectIrRepositoryAction, lager::deps<IrController &>>;
using ProjectIrRepositoryContext = ProjectIrRepositoryResult::effect_t::context_t;

ProjectIrRepositoryResult UpdateProjectIrRepository (ProjectIrRepositoryModel model,
                                                     ProjectIrRepositoryAction action);
