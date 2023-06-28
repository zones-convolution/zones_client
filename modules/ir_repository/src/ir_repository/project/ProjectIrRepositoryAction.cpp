#include "ProjectIrRepositoryAction.h"

#include <iostream>
#include <juce_core/juce_core.h>

ProjectIrRepositoryEffect string_effect = [] (const ProjectIrRepositoryEffect::context_t & ctx)
{
    ctx.dispatch (RemoveProjectPathAction {});
    auto & string = lager::get<std::string> (ctx);
    DBG (string);
};

ProjectIrRepositoryResult Update (ProjectIrRepositoryModel model, ProjectIrRepositoryAction action)
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
                return {model, string_effect};
            }},
        action);
}
