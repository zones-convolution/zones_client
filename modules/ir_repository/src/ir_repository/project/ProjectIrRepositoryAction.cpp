#include "ProjectIrRepositoryAction.h"

#include <iostream>
#include <juce_core/juce_core.h>

// ProjectIrRepositoryEffect string_effect = [] (const ProjectIrRepositoryEffect::context_t & ctx)
//{
//     ctx.dispatch (RemoveProjectPathAction {});
//     auto & string = lager::get<std::string> (ctx);
//     DBG (string);
// };

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
                model.import_project_ir = import_project_ir_action.import_project_ir;
                model.importing_project_ir_state = ProjectIrLoadingState::kPending;
                return {model, lager::noop};
            },
            [&] (const ImportProjectIrLoadingAction &) -> ProjectIrRepositoryResult
            {
                model.import_project_ir = std::nullopt;
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
                model.current_project_ir = load_project_ir_action.ir_identifier;
                model.current_project_ir_state = ProjectIrLoadingState::kPending;
                return {model, lager::noop};
            },
            [&] (const LoadProjectIrLoadingAction &) -> ProjectIrRepositoryResult
            {
                model.current_project_ir_state = ProjectIrLoadingState::kLoading;
                return {model, lager::noop};
            },
            [&] (const LoadProjectIrSuccessAction &) -> ProjectIrRepositoryResult
            {
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
