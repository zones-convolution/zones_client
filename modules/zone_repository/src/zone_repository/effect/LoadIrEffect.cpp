#include "LoadIrEffect.h"

#include "ir_engine/IrController.h"

void LoadIrEffect (const ZoneRepositoryModel & model,
                   const LoadIrAction & action,
                   const ZoneRepositoryContext & context)
{
    context.loop ().async (
        [context, action] ()
        {
            try
            {
                auto & ir_controller = lager::get<IrController> (context);
                ir_controller.LoadIr (action.ir_selection);
                context.dispatch (LoadIrSuccessAction {.ir = action.ir_selection});
                return;
            }
            catch (...)
            {
                context.dispatch (LoadIrFailureAction {});
            }
        });
}