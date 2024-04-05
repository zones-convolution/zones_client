#include "LoadProjectIrEffect.h"

#include "../ProjectIrPaths.h"
#include "ir_engine/IrController.h"
#include "ir_format/io/IrReader.h"

static void LoadProjectIr (const std::filesystem::path & project_path,
                           const std::string & ir_identifier,
                           IrController & ir_controller)
{
    IrReader ir_reader;
    IrData ir_data;
    ir_reader.ReadIrData (project_path, ir_identifier, ir_data);
    auto ir_metadata = ir_reader.ReadIrMetadata (project_path, ir_identifier);

    ir_controller.LoadIr (ir_metadata, ir_data);
}

void LoadProjectIrEffect (const ProjectIrRepositoryModel & model,
                          const std::string & ir_identifier,
                          const ProjectIrRepositoryContext & context)
{
    context.dispatch (LoadProjectIrLoadingAction {});

    auto project_ir_path = GetAvailableProjectPath (model);
    if (! project_ir_path.has_value ())
    {
        context.dispatch (LoadProjectIrFailureAction {});
        return;
    }

    context.loop ().async (
        [context, ir_identifier, project_ir_path] ()
        {
            try
            {
                auto & ir_controller = lager::get<IrController> (context);
                LoadProjectIr (project_ir_path.value (), ir_identifier, ir_controller);
                context.dispatch (LoadProjectIrSuccessAction {.ir_identifier = ir_identifier});
            }
            catch (...)
            {
                context.dispatch (LoadProjectIrFailureAction {});
            }
        });
}