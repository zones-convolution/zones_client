#include "IrRepositoryAction.h"

#include "effect/LoadIrEffect.h"
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
            [&] (const LoadIrAction & load_ir_action) -> IrRepositoryResult
            {
                model.ir_loading_state = IrLoadingState::kLoading;
                return {model, [model, load_ir_action] (auto && context) {
                            LoadIrEffect (model, load_ir_action, context);
                        }};
            },
            [&] (const LoadIrSuccessAction & load_ir_success_action) -> IrRepositoryResult
            {
                model.current_ir_metadata = load_ir_success_action.ir_metadata;
                model.ir_loading_state = IrLoadingState::kSuccess;
                return {model, lager::noop};
            },
            [&] (const LoadIrFailureAction &) -> IrRepositoryResult
            {
                model.ir_loading_state = IrLoadingState::kFailure;
                return {model, lager::noop};
            },
        },
        action);
}