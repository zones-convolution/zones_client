#include "Action.h"

Result Update (Model model, Action action)
{
    return std::visit (
        lager::visitor {[&] (const IrRepositoryAction & ir_repository_action) -> Result
                        {
                            auto [updated_model, effect] = UpdateIrRepository (
                                model.ir_repository_model, ir_repository_action);
                            model.ir_repository_model = updated_model;
                            return {model, effect};
                        },
                        [&] (const IrLoadingAction & ir_loading_action) -> Result
                        {
                            auto [updated_model, effect] =
                                UpdateIrLoading (model.ir_loading_model, ir_loading_action);
                            model.ir_loading_model = updated_model;
                            return {model, effect};
                        }},
        action);
}
