#include "ir_repository/project/ProjectIrRepositoryAction.h"
#include "ir_repository/project/ProjectIrRepositoryModel.h"

#include <catch2/catch_test_macros.hpp>
#include <immer/flex_vector_transient.hpp>

static void RequireProjectPathsIncludes (const ProjectIrRepositoryModel & model,
                                         const std::filesystem::path & project_path)
{
    auto begin = model.project_paths.begin ();
    auto end = model.project_paths.end ();
    auto find_iterator = std::find (begin, end, project_path);
    REQUIRE (find_iterator != end);
}

TEST_CASE ("add and remove project path actions")
{
    SECTION ("add project paths")
    {
        const std::filesystem::path new_project_path = "new/project/path";

        auto model = ProjectIrRepositoryModel {};
        auto update_result =
            Update (model, AddProjectPathAction {.project_path = new_project_path});
        model = update_result.first;

        REQUIRE (model.project_paths.size () == 1);
        RequireProjectPathsIncludes (model, new_project_path);

        const std::filesystem::path another_new_project_path = "another/new/project/path";
        update_result =
            Update (model, AddProjectPathAction {.project_path = another_new_project_path});
        model = update_result.first;

        REQUIRE (model.project_paths.size () == 2);
        RequireProjectPathsIncludes (model, new_project_path);
        RequireProjectPathsIncludes (model, another_new_project_path);
    }

    SECTION ("remove existing project path")
    {
        auto model = ProjectIrRepositoryModel {.project_paths = {"remove_path"}};
        REQUIRE (model.project_paths.size () == 1);
        auto update_result = Update (model, RemoveProjectPathAction {.remove_at_index = 0});
        model = update_result.first;
        REQUIRE (model.project_paths.empty ());
    }

    SECTION ("remove non existing project path")
    {
        auto model = ProjectIrRepositoryModel {.project_paths = {"remove_path"}};
        REQUIRE (model.project_paths.size () == 1);
        auto update_result = Update (model, RemoveProjectPathAction {.remove_at_index = 10});
        model = update_result.first;
        REQUIRE (model.project_paths.size () == 1);
    }
}
