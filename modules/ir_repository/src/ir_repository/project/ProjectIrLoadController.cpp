#include "ProjectIrLoadController.h"

#include "ProjectIrPaths.h"

ProjectIrLoadController::ProjectIrLoadController (
    const lager::reader<ProjectIrRepositoryModel> & project_ir_reader,
    lager::context<ProjectIrRepositoryAction> context,
    IrReader & ir_reader)
    : project_ir_reader_ (project_ir_reader)
    , context_ (std::move (context))
    , current_project_ir_reader_ (
          ProjectIrRepositoryModel::CurrentProjectIrReader (project_ir_reader))
    , ir_reader_ (ir_reader)
{
    lager::watch (current_project_ir_reader_,
                  [&] (CurrentProjectIrOptional current_project_ir)
                  {
                      if (! current_project_ir.has_value ())
                          return;

                      context_.dispatch (LoadProjectIrLoadingAction {});

                      auto project_ir_path =
                          ProjectIrPaths (project_ir_reader_).GetAvailableProjectPath ();
                      if (! project_ir_path.has_value ())
                      {
                          context_.dispatch (LoadProjectIrFailureAction {});
                          return;
                      }

                      try
                      {
                          LoadProjectIr (project_ir_path.value (), current_project_ir.value ());
                          context_.dispatch (LoadProjectIrSuccessAction {});
                      }
                      catch (...)
                      {
                          context_.dispatch (LoadProjectIrFailureAction {});
                      }
                  });
}

const IrData * ProjectIrLoadController::GetCurrentProjectIrData () const
{
    return &current_project_ir_data_;
}

const IrMetadata & ProjectIrLoadController::GetCurrentProjectIrMetadata () const
{
    return current_project_ir_metadata_;
}

void ProjectIrLoadController::LoadProjectIr (const std::filesystem::path & project_path,
                                             const std::string & ir_identifier)
{
    ir_reader_.ReadIrData (project_path, ir_identifier, current_project_ir_data_);
    current_project_ir_metadata_ = ir_reader_.ReadIrMetadata (project_path, ir_identifier);
}
