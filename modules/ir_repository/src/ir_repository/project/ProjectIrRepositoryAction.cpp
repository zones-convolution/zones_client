#include "ProjectIrRepositoryAction.h"

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
