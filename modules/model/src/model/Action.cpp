#include "Action.h"

Result Update (Model model, Action action)
{
    return std::visit (
        lager::visitor {
            [&] (const ProjectIrRepositoryAction & project_ir_repository_action) -> Result
            {
                auto [updated_model, effect] = UpdateProjectIrRepository (
                    model.project_ir_repository_model, project_ir_repository_action);
                model.project_ir_repository_model = updated_model;
                return {model, effect};
            },
            [&] (const AccountAction & account_action) -> Result
            {
                auto [updated_model, effect] = UpdateAccount (model.account_model, account_action);
                model.account_model = updated_model;
                return {model, effect};
            }},
        action);
}
