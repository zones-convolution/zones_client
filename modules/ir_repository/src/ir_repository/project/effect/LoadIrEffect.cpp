#include "LoadIrEffect.h"

#include "ir_engine/IrController.h"

void LoadIrEffect (const IrLoadingModel & model,
                   const LoadIrAction & load_ir_action,
                   const IrLoadingContext & context)
{
    if (load_ir_action.search_paths.empty ())
    {
        context.dispatch (LoadIrFailureAction {});
        return;
    }

    context.loop ().async (
        [context, load_ir_action] ()
        {
            try
            {
                auto & ir_controller = lager::get<IrController> (context);

                auto & search_paths = load_ir_action.search_paths;
                auto & ir_path = load_ir_action.ir_path;

                for (auto & search_path : search_paths)
                {
                    auto absolute_ir_path = search_path / ir_path;
                    if (! std::filesystem::exists (absolute_ir_path))
                        continue;

                    ir_controller.LoadIr (absolute_ir_path);
                    context.dispatch (LoadIrSuccessAction {.ir_path = ir_path});
                    return;
                }

                context.dispatch (LoadIrFailureAction {});
            }
            catch (...)
            {
                context.dispatch (LoadIrFailureAction {});
            }
        });
}