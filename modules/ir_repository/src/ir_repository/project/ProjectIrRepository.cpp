#include "ProjectIrRepository.h"

ProjectIrRepository::ProjectIrRepository (lager::reader<ProjectIrRepositoryModel> reader,
                                          lager::context<ProjectIrRepositoryAction> context,
                                          ProjectPathPickerDelegate & project_path_picker_delegate,
                                          IrReader & ir_reader,
                                          IrWriter & ir_writer)
    : reader_ (reader)
    , project_path_picker_delegate_ (project_path_picker_delegate)
    , ir_reader_ (ir_reader)
    , ir_writer_ (ir_writer)
{
}

void ProjectIrRepository::LoadNewProjectIr (const std::filesystem::path & ir_path,
                                            const std::string & name,
                                            const std::string & description,
                                            ProjectIrRepository::LoadNewProjectIrCallback callback)
{
    //    auto load_ir = [&] (const std::filesystem::path & project_path)
    //    {
    //        TransferIrToProject (ir_path, project_path, name, description);
    //        callback (name);
    //    };

    // auto project_path = GetAvailableProjectPath ();
    //
    //    if (! project_path.has_value ())
    //        project_path_picker_delegate_.PickPath ([&] (ProjectPathPickerDelegate::Result
    //        path_result)
    //                                                { load_ir (path_result.path); });
    //    else
    //        load_ir (project_path.value ());
}

IrMetadata ProjectIrRepository::LoadIrMetaData (const std::string & ir_identifier)
{
    return IrMetadata ();
}

void ProjectIrRepository::LoadIrData (const std::string & ir_identifier, IrData & ir_data)
{
}
