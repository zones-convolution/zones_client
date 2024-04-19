#include "IrRepositoryAction.h"

#include "effect/RefreshUserIrsEffect.h"

IrRepositoryResult UpdateIrRepository (IrRepositoryModel model, IrRepositoryAction action)
{
    return std::visit (
        lager::visitor {

            [&] (const RefreshUserIrsAction & action) -> IrRepositoryResult
            {
                model.user_irs_loading = true;
                return {model, [model, action] (auto && context) {
                            RefreshUserIrsEffect (action, context);
                        }};
            },
            [&] (const RefreshUserIrsResultAction & action) -> IrRepositoryResult
            {
                model.user_irs = action.user_irs;
                model.user_irs_loading = false;
                return {model, lager::noop};
            },
        },
        action);
}