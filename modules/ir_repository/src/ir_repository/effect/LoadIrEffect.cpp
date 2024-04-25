#include "LoadIrEffect.h"

#include "ir_engine/IrController.h"

void LoadIrEffect (const IrRepositoryModel & model,
                   const LoadIrAction & action,
                   const IrRepositoryContext & context)
{
    context.loop ().async (
        [context, action] ()
        {
            try
            {
                auto & ir_controller = lager::get<IrController> (context);
                ir_controller.LoadIr (action.ir_metadata, action.target_format);
                context.dispatch (LoadIrSuccessAction {.ir_metadata = action.ir_metadata});
                return;
            }
            catch (...)
            {
                context.dispatch (LoadIrFailureAction {});
            }
        });
}