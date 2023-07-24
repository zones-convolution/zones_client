#include "LagerTestUtils.h"
#include "ReaderWriterMocks.h"
#include "ir_repository/project/ProjectIrLoadController.h"
#include "ir_repository/project/ProjectIrRepositoryAction.h"
#include "ir_repository/project/ProjectIrRepositoryModel.h"

#include <catch2/catch_test_macros.hpp>
#include <lager/state.hpp>

TEST_CASE ("loads project ir", "[ProjectIrLoadController]")
{
    SECTION ("dispatches failure action when no valid project paths exist")
    {
        IrReaderMock ir_reader_mock;
        auto model_state = lager::make_state (ProjectIrRepositoryModel {}, lager::automatic_tag {});
        TrackingContext<ProjectIrRepositoryAction> tracking_context;

        ProjectIrLoadController project_ir_load_controller {
            model_state, tracking_context.context_, ir_reader_mock};

        auto [model, effect] = UpdateProjectIrRepository (
            model_state.get (), LoadProjectIrAction {.ir_identifier = "invalid_ir_identifier"});
        model_state.set (model);

        RequireDispatchedAt<ProjectIrRepositoryAction, LoadProjectIrLoadingAction> (
            0, tracking_context.dispatched_actions_);

        RequireDispatchedAt<ProjectIrRepositoryAction, LoadProjectIrFailureAction> (
            1, tracking_context.dispatched_actions_);
    }

    SECTION ("dispatches success action when ir is found in project")
    {
        IrReaderMock ir_reader_mock;
        ir_reader_mock.ir_metadata_ = IrMetadata {.name = "ir_name"};

        auto valid_dir =
            juce::File::getSpecialLocation (juce::File::SpecialLocationType::tempDirectory);

        auto model_state = lager::make_state (
            ProjectIrRepositoryModel {
                .project_paths = {valid_dir.getFullPathName ().toStdString ()}},
            lager::automatic_tag {});
        TrackingContext<ProjectIrRepositoryAction> tracking_context;

        ProjectIrLoadController project_ir_load_controller {
            model_state, tracking_context.context_, ir_reader_mock};

        auto [model, effect] = UpdateProjectIrRepository (
            model_state.get (), LoadProjectIrAction {.ir_identifier = "invalid_ir_identifier"});
        model_state.set (model);

        RequireDispatchedAt<ProjectIrRepositoryAction, LoadProjectIrLoadingAction> (
            0, tracking_context.dispatched_actions_);

        RequireDispatchedAt<ProjectIrRepositoryAction, LoadProjectIrSuccessAction> (
            1, tracking_context.dispatched_actions_);

        REQUIRE (ir_reader_mock.did_read_ir_data_);
        REQUIRE (ir_reader_mock.did_read_ir_metadata_);

        auto & loaded_metadata = project_ir_load_controller.GetCurrentProjectIrMetadata ();
        REQUIRE (loaded_metadata.name == ir_reader_mock.ir_metadata_.name);
    }
}