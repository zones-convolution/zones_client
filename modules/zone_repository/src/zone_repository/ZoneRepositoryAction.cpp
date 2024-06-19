#include "ZoneRepositoryAction.h"

#include "effect/LoadIrEffect.h"

ZoneRepositoryResult UpdateZoneRepository (ZoneRepositoryModel model,
                                           ZoneRepositoryAction zone_repository_action)
{
    return std::visit (
        lager::visitor {
            [&] (const RefreshValidTargetFormatsAction & action) -> ZoneRepositoryResult
            {
                model.valid_target_formats = action.target_formats;
                return {model, lager::noop};
            },
            [&] (const LoadIrAction & load_ir_action) -> ZoneRepositoryResult
            {
                model.ir_loading_state = IrLoadingState::kLoading;
                return {model, [model, load_ir_action] (auto && context) {
                            LoadIrEffect (model, load_ir_action, context);
                        }};
            },
            [&] (const LoadIrSuccessAction & load_ir_success_action) -> ZoneRepositoryResult
            {
                model.current_ir = load_ir_success_action.ir;
                model.ir_loading_state = IrLoadingState::kSuccess;
                return {model, lager::noop};
            },
            [&] (const LoadIrFailureAction &) -> ZoneRepositoryResult
            {
                model.ir_loading_state = IrLoadingState::kFailure;
                return {model, lager::noop};
            },
        },
        zone_repository_action);
}