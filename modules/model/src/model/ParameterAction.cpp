#include "ParameterAction.h"

template <typename ParameterModel, typename ParameterAction>
ParameterModel UpdateParameter (ParameterModel parameter_model, ParameterAction parameter_action)
{
    return std::visit (
        lager::visitor {
            [&] (const UpdateParameterAction<ParameterModel, float> & update_parameter_action)
            {
                parameter_model.*update_parameter_action.member_ptr = update_parameter_action.value;
                return parameter_model;
            }},
        parameter_action);
}

IrEngineParameterModel UpdateIrEngineParameter (IrEngineParameterModel model,
                                                IrEngineParameterAction action)
{
    return UpdateParameter (model, action);
}

RealtimeParameterModel UpdateRealtimeParameter (RealtimeParameterModel model,
                                                RealtimeParameterAction action)
{
    return UpdateParameter (model, action);
}
