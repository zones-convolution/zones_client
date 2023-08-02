#include "ParameterAction.h"

ParameterModel UpdateParameter (ParameterModel model, ParameterAction action)
{
    return std::visit (
        lager::visitor {[&] (const UpdateParameterAction<float> & update_parameter_action)
                        {
                            model.*update_parameter_action.member_ptr =
                                update_parameter_action.value;
                            return model;
                        }},
        action);
}
