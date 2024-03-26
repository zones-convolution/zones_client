#include "ImportProjectIrEffect.h"

#include "../../io/IrReader.h"
#include "../../io/IrWriter.h"
#include "../ProjectIrPaths.h"

static void TransferIrToProject (const std::filesystem::path & project_path,
                                 const ImportProjectIr & import_project_ir)
{
    IrReader ir_reader;
    IrWriter ir_writer;

    IrData ir_data;
    IrMetadata ir_metadata {.name = import_project_ir.name,
                            .description = import_project_ir.description};

    auto load_path = import_project_ir.ir_path.parent_path ();
    auto original_identifier = import_project_ir.ir_path.stem ();

    ir_reader.ReadIrData (load_path, original_identifier, ir_data);

    auto ir_identifier = import_project_ir.name;
    ir_writer.WriteIrData (project_path, ir_identifier, ir_data);
    ir_writer.WriteIrMetadata (project_path, ir_identifier, ir_metadata);
}

void ImportProjectIrEffect (const ProjectIrRepositoryModel & model,
                            const ImportProjectIr & import_project_ir,
                            const ProjectIrRepositoryContext & context)
{
    context.dispatch (ImportProjectIrLoadingAction {});
    auto project_ir_path = GetAvailableProjectPath (model);

    if (! project_ir_path.has_value ())
    {
        context.dispatch (ImportProjectIrFailureAction {});
        return;
    }

    context.loop ().async (
        [context, import_project_ir, project_ir_path] ()
        {
            try
            {
                TransferIrToProject (project_ir_path.value (), import_project_ir);
                context.dispatch (ImportProjectIrSuccessAction {});
            }
            catch (...)
            {
                context.dispatch (ImportProjectIrFailureAction {});
            }
        });
}
