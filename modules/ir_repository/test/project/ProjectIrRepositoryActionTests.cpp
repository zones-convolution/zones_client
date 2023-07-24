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
        auto update_result = UpdateProjectIrRepository (
            model, AddProjectPathAction {.project_path = new_project_path});
        model = update_result.first;

        REQUIRE (model.project_paths.size () == 1);
        RequireProjectPathsIncludes (model, new_project_path);

        const std::filesystem::path another_new_project_path = "another/new/project/path";
        update_result = UpdateProjectIrRepository (
            model, AddProjectPathAction {.project_path = another_new_project_path});
        model = update_result.first;

        REQUIRE (model.project_paths.size () == 2);
        RequireProjectPathsIncludes (model, new_project_path);
        RequireProjectPathsIncludes (model, another_new_project_path);
    }

    SECTION ("remove existing project path")
    {
        auto model = ProjectIrRepositoryModel {.project_paths = {"remove_path"}};
        REQUIRE (model.project_paths.size () == 1);
        auto update_result =
            UpdateProjectIrRepository (model, RemoveProjectPathAction {.remove_at_index = 0});
        model = update_result.first;
        REQUIRE (model.project_paths.empty ());
    }

    SECTION ("remove non existing project path")
    {
        auto model = ProjectIrRepositoryModel {.project_paths = {"remove_path"}};
        REQUIRE (model.project_paths.size () == 1);
        auto update_result =
            UpdateProjectIrRepository (model, RemoveProjectPathAction {.remove_at_index = 10});
        model = update_result.first;
        REQUIRE (model.project_paths.size () == 1);
    }
}

TEST_CASE ("project ir import actions")
{
    SECTION ("import action updates target ir and sets state to pending")
    {
        auto model = ProjectIrRepositoryModel {.importing_project_ir_state =
                                                   ProjectIrLoadingState::kSuccess};
        auto import_action = ImportProjectIrAction {
            .import_project_ir = {
                .ir_path = "path/to/ir.wav", .name = "ir_name", .description = "ir_description"}};
        auto update_result = UpdateProjectIrRepository (model, import_action);
        model = update_result.first;

        REQUIRE (model.import_project_ir.has_value ());
        REQUIRE (model.import_project_ir->ir_path == import_action.import_project_ir.ir_path);
        REQUIRE (model.import_project_ir->name == import_action.import_project_ir.name);
        REQUIRE (model.import_project_ir->description ==
                 import_action.import_project_ir.description);
        REQUIRE (model.importing_project_ir_state == ProjectIrLoadingState::kPending);
    }

    SECTION ("loading action sets loading state and clears target ir")
    {
        auto model = ProjectIrRepositoryModel {.importing_project_ir_state =
                                                   ProjectIrLoadingState::kSuccess};
        auto update_result = UpdateProjectIrRepository (model, ImportProjectIrLoadingAction {});
        model = update_result.first;

        REQUIRE (model.import_project_ir == std::nullopt);
        REQUIRE (model.importing_project_ir_state == ProjectIrLoadingState::kLoading);
    }

    SECTION ("success action sets success state")
    {
        auto model = ProjectIrRepositoryModel {.importing_project_ir_state =
                                                   ProjectIrLoadingState::kLoading};
        auto update_result = UpdateProjectIrRepository (model, ImportProjectIrSuccessAction {});
        model = update_result.first;

        REQUIRE (model.importing_project_ir_state == ProjectIrLoadingState::kSuccess);
    }

    SECTION ("failure action sets failure state")
    {
        auto model = ProjectIrRepositoryModel {.importing_project_ir_state =
                                                   ProjectIrLoadingState::kLoading};
        auto update_result = UpdateProjectIrRepository (model, ImportProjectIrFailureAction {});
        model = update_result.first;

        REQUIRE (model.importing_project_ir_state == ProjectIrLoadingState::kFailure);
    }
}

TEST_CASE ("project ir load actions")
{
    SECTION ("set load action updates ir to load and sets state to pending")
    {
        auto model = ProjectIrRepositoryModel {};
        auto load_project_ir_action = LoadProjectIrAction {.ir_identifier = "ir_identifier"};
        auto update_result = UpdateProjectIrRepository (model, load_project_ir_action);
        model = update_result.first;

        REQUIRE (model.current_project_ir.has_value ());
        REQUIRE (model.current_project_ir == load_project_ir_action.ir_identifier);
        REQUIRE (model.current_project_ir_state == ProjectIrLoadingState::kPending);
    }

    SECTION ("loading action sets loading state")
    {
        auto model =
            ProjectIrRepositoryModel {.current_project_ir_state = ProjectIrLoadingState::kPending};
        auto update_result = UpdateProjectIrRepository (model, LoadProjectIrLoadingAction {});
        model = update_result.first;

        REQUIRE (model.current_project_ir_state == ProjectIrLoadingState::kLoading);
    }
    SECTION ("success action sets success state")
    {
        auto model =
            ProjectIrRepositoryModel {.current_project_ir_state = ProjectIrLoadingState::kLoading};
        auto update_result = UpdateProjectIrRepository (model, LoadProjectIrSuccessAction {});
        model = update_result.first;

        REQUIRE (model.current_project_ir_state == ProjectIrLoadingState::kSuccess);
    }
    SECTION ("failure action sets failure state")
    {
        auto model =
            ProjectIrRepositoryModel {.current_project_ir_state = ProjectIrLoadingState::kLoading};
        auto update_result = UpdateProjectIrRepository (model, LoadProjectIrFailureAction {});
        model = update_result.first;

        REQUIRE (model.current_project_ir_state == ProjectIrLoadingState::kFailure);
    }
}
