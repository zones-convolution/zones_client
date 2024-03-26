#include "ProjectIrRepositoryAction.h"

#include "effect/ImportProjectIrEffect.h"
#include "effect/LoadProjectIrEffect.h"

ProjectIrRepositoryResult UpdateProjectIrRepository (ProjectIrRepositoryModel model,
                                                     ProjectIrRepositoryAction action)
{
    return std::visit (
        lager::visitor {
            [&] (const AddProjectPathAction & add_project_path_action) -> ProjectIrRepositoryResult
            {
                model.project_paths =
                    model.project_paths.push_back (add_project_path_action.project_path);
                return {model, lager::noop};
            },
            [&] (const RemoveProjectPathAction & remove_project_path_action)
                -> ProjectIrRepositoryResult
            {
                model.project_paths =
                    model.project_paths.erase (remove_project_path_action.remove_at_index);
                return {model, lager::noop};
            },
            [&] (
                const ImportProjectIrAction & import_project_ir_action) -> ProjectIrRepositoryResult
            {
                model.importing_project_ir_state = ProjectIrLoadingState::kPending;
                return {model,
                        [import_project_ir = import_project_ir_action.import_project_ir,
                         model] (auto && context)
                        { ImportProjectIrEffect (model, import_project_ir, context); }};
            },
            [&] (const ImportProjectIrLoadingAction &) -> ProjectIrRepositoryResult
            {
                model.importing_project_ir_state = ProjectIrLoadingState::kLoading;
                return {model, lager::noop};
            },
            [&] (const ImportProjectIrSuccessAction &) -> ProjectIrRepositoryResult
            {
                model.importing_project_ir_state = ProjectIrLoadingState::kSuccess;
                return {model, lager::noop};
            },
            [&] (const ImportProjectIrFailureAction &) -> ProjectIrRepositoryResult
            {
                model.importing_project_ir_state = ProjectIrLoadingState::kFailure;
                return {model, lager::noop};
            },
            [&] (const LoadProjectIrAction & load_project_ir_action) -> ProjectIrRepositoryResult
            {
                model.current_project_ir_state = ProjectIrLoadingState::kPending;
                return {
                    model,
                    [model, ir_identifier = load_project_ir_action.ir_identifier] (auto && context)
                    { LoadProjectIrEffect (model, ir_identifier, context); }};
            },
            [&] (const LoadProjectIrLoadingAction &) -> ProjectIrRepositoryResult
            {
                model.current_project_ir_state = ProjectIrLoadingState::kLoading;
                return {model, lager::noop};
            },
            [&] (const LoadProjectIrSuccessAction & load_project_ir_success_action)
                -> ProjectIrRepositoryResult
            {
                model.current_project_ir = load_project_ir_success_action.ir_identifier;
                model.current_project_ir_state = ProjectIrLoadingState::kSuccess;
                return {model, lager::noop};
            },
            [&] (const LoadProjectIrFailureAction &) -> ProjectIrRepositoryResult
            {
                model.current_project_ir_state = ProjectIrLoadingState::kFailure;
                return {model, lager::noop};
            },
        },
        action);
}