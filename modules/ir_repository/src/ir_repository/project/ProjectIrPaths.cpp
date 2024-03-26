#include "ProjectIrPaths.h"

#include <juce_core/juce_core.h>

bool IsPathValidDirectory (const std::filesystem::path & path)
{
    juce::File file_path (path.string ());
    return file_path.exists () and file_path.isDirectory ();
}

std::optional<std::filesystem::path>
GetAvailableProjectPath (const ProjectIrRepositoryModel & model)
{
    for (auto & project_path : model.project_paths)
        if (IsPathValidDirectory (project_path))
            return project_path;

    return std::nullopt;
}