#include "ProjectIrPaths.h"

#include <juce_core/juce_core.h>

bool IsPathValidDirectory (const std::filesystem::path & path)
{
    juce::File file_path (path.string ());
    return file_path.exists () and file_path.isDirectory ();
}

ProjectIrPaths::ProjectIrPaths (lager::reader<ProjectIrRepositoryModel> reader)
    : reader_ (reader)
{
}

std::optional<std::filesystem::path> ProjectIrPaths::GetAvailableProjectPath ()
{
    auto model = reader_.get ();

    for (auto & project_path : model.project_paths)
        if (IsPathValidDirectory (project_path))
            return project_path;

    return std::nullopt;
}
