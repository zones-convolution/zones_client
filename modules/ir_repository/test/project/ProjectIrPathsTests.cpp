
#include "ir_repository/project/ProjectIrPaths.h"
#include "ir_repository/project/ProjectIrRepositoryModel.h"

#include <catch2/catch_test_macros.hpp>
#include <juce_core/juce_core.h>
#include <lager/state.hpp>

TEST_CASE ("can retrieve first available project path", "[ProjectIrPaths]")
{
    SECTION ("returns nulloptional when no project paths are available")
    {
        auto first_available_path = GetAvailableProjectPath (ProjectIrRepositoryModel {});
        REQUIRE (! first_available_path.has_value ());
    }

    SECTION ("returns nulloptional when no valid project paths are available")
    {
        auto first_available_path = GetAvailableProjectPath (ProjectIrRepositoryModel {
            .project_paths = {"invalid/directory_1", "invalid/directory_2"}});
        REQUIRE (! first_available_path.has_value ());
    }

    SECTION ("valid project paths")
    {
        auto valid_dir =
            juce::File::getSpecialLocation (juce::File::SpecialLocationType::tempDirectory)
                .getChildFile ("valid_dir");
        valid_dir.createDirectory ();

        SECTION ("returns valid project path when a single valid project path is available")
        {
            auto first_available_path = GetAvailableProjectPath (ProjectIrRepositoryModel {
                .project_paths = {valid_dir.getFullPathName ().toStdString ()}});
            REQUIRE (first_available_path.value () == valid_dir.getFullPathName ().toStdString ());
        }

        SECTION ("returns valid project path when valid and invalid project paths are available")
        {
            auto first_available_path = GetAvailableProjectPath (ProjectIrRepositoryModel {
                .project_paths = {"invalid/project/path",
                                  valid_dir.getFullPathName ().toStdString ()}});
            REQUIRE (first_available_path.value () == valid_dir.getFullPathName ().toStdString ());
        }

        /**
         * Potential issue raised regarding cleaning up resources in tests that may exist before
         * cleaning up This has been left for now, related to the temp directory so possibly not
         * an issue
         */
        valid_dir.moveToTrash ();
    }
}