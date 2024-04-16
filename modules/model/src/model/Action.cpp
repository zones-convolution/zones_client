#include "Action.h"

Result Update (Model model, Action action)
{
    return std::visit (lager::visitor {[&] (const IrLoadingAction & ir_loading_action) -> Result
                                       {
                                           auto [updated_model, effect] = UpdateIrLoading (
                                               model.ir_loading_model, ir_loading_action);
                                           model.ir_loading_model = updated_model;
                                           return {model, effect};
                                       },
                                       [&] (const AccountAction & account_action) -> Result
                                       {
                                           auto [updated_model, effect] =
                                               UpdateAccount (model.account_model, account_action);
                                           model.account_model = updated_model;
                                           return {model, effect};
                                       }},
                       action);
}
