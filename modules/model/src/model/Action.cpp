#include "Action.h"

Model Update (Model model, Action action)
{
    return std::visit (
        lager::visitor {[&] (const RealtimeParameterAction & parameter_action)
                        {
                            model.realtime_parameter_model = UpdateRealtimeParameter (
                                model.realtime_parameter_model, parameter_action);
                            return model;
                        },
                        [&] (const IrEngineParameterAction & parameter_action)
                        {
                            model.ir_engine_parameter_model = UpdateIrEngineParameter (
                                model.ir_engine_parameter_model, parameter_action);
                            return model;
                        },
                        [&] (const ProjectIrRepositoryAction & project_ir_repository_action)
                        {
                            model.project_ir_repository_model =
                                UpdateProjectIrRepository (model.project_ir_repository_model,
                                                           project_ir_repository_action)
                                    .first;
                            return model;
                        }},
        action);
}
