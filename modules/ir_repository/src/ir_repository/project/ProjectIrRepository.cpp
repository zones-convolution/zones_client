#include "ProjectIrRepository.h"

ProjectIrRepository::ProjectIrRepository (lager::reader<ProjectIrRepositoryModel> reader,
                                          lager::context<ProjectIrRepositoryAction> context,
                                          ProjectIrPickerDelegate & project_ir_picker_delegate,
                                          ProjectPathPickerDelegate & project_path_picker_delegate,
                                          IrReader & ir_reader,
                                          IrWriter & ir_writer)
    : reader_ (reader)
    , project_ir_picker_delegate_ (project_ir_picker_delegate)
    , project_path_picker_delegate_ (project_path_picker_delegate)
    , ir_reader_ (ir_reader)
    , ir_writer_ (ir_writer)
{
}

// void ProjectIrRepository::LinkProjectPath (const std::filesystem::path & project_path)
//{
// }
//
//  std::optional<std::filesystem::path> ProjectIrRepository::GetProjectPath ()
//{
//     auto model = reader_.get ();
//
//     for (auto & project_path : model.project_paths)
//         if (IsPathValidDirectory (project_path))
//             return project_path;
//
//     return std::nullopt;
// }

void ProjectIrRepository::LoadNewProjectIr (ProjectIrRepository::LoadNewProjectIrCallback callback)
{
    //    auto pick_ir_callback = [&] (ProjectIrPickerDelegate::Result ir_result)
    //    {
    //        auto load_ir = [&] (std::filesystem::path project_path)
    //        {
    //            IrData ir_data;
    //            IrMetadata ir_metadata {.name = ir_result.name, .description =
    //            ir_result.description}; auto load_path = ir_result.path.parent_path (); auto
    //            original_identifier = ir_result.path.stem (); ir_reader_.ReadIrData (load_path,
    //            original_identifier, ir_data);
    //
    //            ir_writer_.WriteIrData (project_path, original_identifier, ir_data);
    //            ir_writer_.WriteIrMetadata (project_path, original_identifier, ir_metadata);
    //
    //            callback (original_identifier);
    //        };
    //
    //        auto project_path = GetProjectPath ();
    //        if (! project_path.has_value ())
    //            project_path_picker_delegate_.PickPath (
    //                [&] (ProjectPathPickerDelegate::Result path_result)
    //                { load_ir (path_result.path); });
    //        else
    //            load_ir (project_path.value ());
    //    };
    //
    //    project_ir_picker_delegate_.PickIr (pick_ir_callback);
}

// IrMetadata ProjectIrRepository::LoadIrMetaData (const std::string & ir_identifier)
//{
//     return IrMetadata ();
// }
//
// void ProjectIrRepository::LoadIrData (const std::string & ir_identifier, IrData & ir_data)
//{
// }

bool ProjectIrRepository::IsPathValidDirectory (const std::filesystem::path & path)
{
    juce::File file_path (path.string ());
    return file_path.exists () and file_path.isDirectory ();
}
