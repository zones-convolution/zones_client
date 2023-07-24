#include "Action.h"

Model Update (Model model, Action action)
{
    return std::visit (
        lager::visitor {
            [&] (const ParameterAction & parameter_action) -> Model
            {
                model.parameter_model = UpdateParameter (model.parameter_model, parameter_action);
                return model;
            },
            [&] (const ProjectIrRepositoryAction & project_ir_repository_action) -> Model
            {
                model.project_ir_repository_model =
                    UpdateProjectIrRepository (model.project_ir_repository_model,
                                               project_ir_repository_action)
                        .first;
                return model;
            }},
        action);
}
