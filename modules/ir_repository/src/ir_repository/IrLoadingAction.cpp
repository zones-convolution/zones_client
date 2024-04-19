#include "IrLoadingAction.h"

#include "effect/LoadIrEffect.h"

IrLoadingResult UpdateIrLoading (IrLoadingModel model, IrLoadingAction action)
{
    return std::visit (
        lager::visitor {

            [&] (const LoadIrAction & load_ir_action) -> IrLoadingResult
            {
                model.ir_loading_state = IrLoadingState::kLoading;
                return {model, [model, load_ir_action] (auto && context) {
                            LoadIrEffect (model, load_ir_action, context);
                        }};
            },
            [&] (const LoadIrSuccessAction & load_ir_success_action) -> IrLoadingResult
            {
                model.ir_path = load_ir_success_action.ir_path;
                model.ir_loading_state = IrLoadingState::kSuccess;
                return {model, lager::noop};
            },
            [&] (const LoadIrFailureAction &) -> IrLoadingResult
            {
                model.ir_loading_state = IrLoadingState::kFailure;
                return {model, lager::noop};
            },
        },
        action);
}