#include "ProjectIrImportController.h"

#include "ProjectIrPaths.h"

ProjectIrImportController::ProjectIrImportController (
    const lager::reader<ProjectIrRepositoryModel> & project_ir_reader,
    lager::context<ProjectIrRepositoryAction> context,
    IrReader & ir_reader,
    IrWriter & ir_writer)
    : project_ir_reader_ (project_ir_reader)
    , context_ (std::move (context))
    , import_project_ir_reader_ (
          ProjectIrRepositoryModel::ImportProjectIrReader (project_ir_reader))
    , ir_reader_ (ir_reader)
    , ir_writer_ (ir_writer)
{
    lager::watch (
        import_project_ir_reader_,
        [&] (ImportProjectIrOptional import_project_ir)
        {
            if (! import_project_ir.has_value ())
                return;

            context_.dispatch (ImportProjectIrLoadingAction {});
            auto project_ir_path = ProjectIrPaths (project_ir_reader_).GetAvailableProjectPath ();

            if (! project_ir_path.has_value ())
            {
                context_.dispatch (ImportProjectIrFailureAction {});
                return;
            }

            try
            {
                TransferIrToProject (project_ir_path.value (), import_project_ir.value ());
                context_.dispatch (ImportProjectIrSuccessAction {});
            }
            catch (...)
            {
                context_.dispatch (ImportProjectIrFailureAction {});
            }
        });
}

void ProjectIrImportController::TransferIrToProject (const std::filesystem::path & project_path,
                                                     const ImportProjectIr & import_project_ir)
{
    IrData ir_data;
    IrMetadata ir_metadata {.name = import_project_ir.name,
                            .description = import_project_ir.description};

    auto load_path = import_project_ir.ir_path.parent_path ();
    auto original_identifier = import_project_ir.ir_path.stem ();

    ir_reader_.ReadIrData (load_path, original_identifier, ir_data);

    auto ir_identifier = import_project_ir.name;
    ir_writer_.WriteIrData (project_path, ir_identifier, ir_data);
    ir_writer_.WriteIrMetadata (project_path, ir_identifier, ir_metadata);
}
