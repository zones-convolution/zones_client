#include "ProjectIrRepository.h"

#include <juce_core/juce_core.h>
ProjectIrRepository::ProjectIrRepository (lager::reader<ProjectIrRepositoryModel> reader,
                                          lager::context<ProjectIrRepositoryAction> context,
                                          ProjectIrPickerDelegate & project_ir_picker_delegate)
    : reader_ (reader)
    , project_ir_picker_delegate_ (project_ir_picker_delegate)
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

void ProjectIrRepository::LoadNewProjectIr (ProjectIrRepository::LoadNewProjectIrCallback callback)
{
    project_ir_picker_delegate_.PickIr (
        [&] (ProjectIrPickerDelegate::Result result)
        {
            auto project_path = GetProjectPath ();

            if (! project_path.has_value ())
                // LinkProjectPath ()
                return;
            else
            {
            }
        });
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
