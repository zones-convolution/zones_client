#include "ProjectIrImportController.h"

#include "ProjectIrPaths.h"

ProjectIrImportController::ProjectIrImportController (
    const lager::reader<ProjectIrRepositoryModel> & model,
    lager::context<ProjectIrRepositoryAction> & context,
    IrReader & ir_reader,
    IrWriter & ir_writer)
    : model_ (model)
    , context_ (context)
    , ir_reader_ (ir_reader)
    , ir_writer_ (ir_writer)
{
    import_project_ir_reader_ =
        model.zoom (lager::lenses::attr (&ProjectIrRepositoryModel::import_project_ir));

    lager::watch (import_project_ir_reader_,
                  [&] (ImportProjectIrOptional import_project_ir)
                  {
                      if (import_project_ir.has_value ())
                      {
                          context.dispatch (ImportProjectIrLoadingAction {});
                          auto project_ir_path = ProjectIrPaths (model_).GetAvailableProjectPath ();
                          if (project_ir_path.has_value ())
                          {
                              try
                              {
                                  TransferIrToProject (project_ir_path.value (),
                                                       import_project_ir.value ());
                                  context.dispatch (ImportProjectIrSuccessAction {});
                              }
                              catch (...)
                              {
                                  context.dispatch (ImportProjectIrFailureAction {});
                              }
                          }
                          else
                          {
                              context.dispatch (ImportProjectIrLoadingAction {});
                          }
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
