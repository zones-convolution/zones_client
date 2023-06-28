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

void ProjectIrRepository::LinkProjectPath (const std::filesystem::path & project_path)
{
}

std::optional<std::filesystem::path> ProjectIrRepository::GetProjectPath ()
{
    auto model = reader_.get ();

    for (auto & project_path : model.project_paths)
        if (IsPathValidDirectory (project_path))
            return project_path;

    return std::nullopt;
}

void ProjectIrRepository::TransferIrToProject (std::filesystem::path original_path,
                                               std::filesystem::path project_path,
                                               std::string name,
                                               std::string description)
{
    IrData ir_data;
    IrMetadata ir_metadata {.name = name, .description = description};

    auto load_path = original_path.parent_path ();
    auto original_identifier = original_path.stem ();

    ir_reader_.ReadIrData (load_path, original_identifier, ir_data);

    auto ir_identifier = name;
    ir_writer_.WriteIrData (project_path, ir_identifier, ir_data);
    ir_writer_.WriteIrMetadata (project_path, ir_identifier, ir_metadata);
}

void ProjectIrRepository::LoadNewProjectIr (const std::filesystem::path & ir_path,
                                            const std::string & name,
                                            const std::string & description,
                                            ProjectIrRepository::LoadNewProjectIrCallback callback)
{
    auto load_ir = [&] (const std::filesystem::path & project_path)
    {
        TransferIrToProject (ir_path, project_path, name, description);
        callback (name);
    };

    auto project_path = GetProjectPath ();

    if (! project_path.has_value ())
        project_path_picker_delegate_.PickPath ([&] (ProjectPathPickerDelegate::Result path_result)
                                                { load_ir (path_result.path); });
    else
        load_ir (project_path.value ());
}

IrMetadata ProjectIrRepository::LoadIrMetaData (const std::string & ir_identifier)
{
    return IrMetadata ();
}

void ProjectIrRepository::LoadIrData (const std::string & ir_identifier, IrData & ir_data)
{
}

bool ProjectIrRepository::IsPathValidDirectory (const std::filesystem::path & path)
{
    juce::File file_path (path.string ());
    return file_path.exists () and file_path.isDirectory ();
}
