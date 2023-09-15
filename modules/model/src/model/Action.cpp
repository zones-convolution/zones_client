#include "Action.h"

Model Update (Model model, Action action)
{
    return std::visit (
        lager::visitor {[&] (const ProjectIrRepositoryAction & project_ir_repository_action)
                        {
                            model.project_ir_repository_model =
                                UpdateProjectIrRepository (model.project_ir_repository_model,
                                                           project_ir_repository_action)
                                    .first;
                            return model;
                        }},
        action);
}
