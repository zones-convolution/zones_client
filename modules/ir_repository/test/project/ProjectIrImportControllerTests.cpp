#include "LagerTestUtils.h"
#include "ReaderWriterMocks.h"
#include "ir_repository/project/ProjectIrImportController.h"
#include "ir_repository/project/ProjectIrRepositoryAction.h"
#include "ir_repository/project/ProjectIrRepositoryModel.h"

#include <catch2/catch_test_macros.hpp>
#include <lager/state.hpp>

TEST_CASE ("imports project ir", "[ProjectIrImportController]")
{
    SECTION ("dispatches failure action when no valid project paths exist")
    {
        IrReaderMock ir_reader_mock;
        IrWriterMock ir_writer_mock;

        auto model_state = lager::make_state (ProjectIrRepositoryModel {}, lager::automatic_tag {});
        TrackingContext<ProjectIrRepositoryAction> tracking_context;

        ProjectIrImportController project_ir_import_controller {
            model_state, tracking_context.context_, ir_reader_mock, ir_writer_mock};

        auto [model, effect] = UpdateProjectIrRepository (
            model_state.get (),
            ImportProjectIrAction {.import_project_ir = {.ir_path = "path/to/ir.wav",
                                                         .name = "name",
                                                         .description = "description"}});
        model_state.set (model);

        RequireDispatchedAt<ProjectIrRepositoryAction, ImportProjectIrLoadingAction> (
            0, tracking_context.dispatched_actions_);

        RequireDispatchedAt<ProjectIrRepositoryAction, ImportProjectIrFailureAction> (
            1, tracking_context.dispatched_actions_);
    }

    SECTION ("dispatches success action when no valid project paths exist")
    {
        IrReaderMock ir_reader_mock;
        IrWriterMock ir_writer_mock;

        auto valid_dir =
            juce::File::getSpecialLocation (juce::File::SpecialLocationType::tempDirectory);

        auto model_state = lager::make_state (
            ProjectIrRepositoryModel {
                .project_paths = {valid_dir.getFullPathName ().toStdString ()}},
            lager::automatic_tag {});

        TrackingContext<ProjectIrRepositoryAction> tracking_context;
        ProjectIrImportController project_ir_import_controller {
            model_state, tracking_context.context_, ir_reader_mock, ir_writer_mock};

        auto [model, effect] = UpdateProjectIrRepository (
            model_state.get (),
            ImportProjectIrAction {.import_project_ir = {.ir_path = "path/to/ir.wav",
                                                         .name = "name",
                                                         .description = "description"}});
        model_state.set (model);

        RequireDispatchedAt<ProjectIrRepositoryAction, ImportProjectIrLoadingAction> (
            0, tracking_context.dispatched_actions_);

        RequireDispatchedAt<ProjectIrRepositoryAction, ImportProjectIrSuccessAction> (
            1, tracking_context.dispatched_actions_);

        REQUIRE (ir_writer_mock.did_write_ir_data_);
        REQUIRE (ir_writer_mock.did_write_ir_metadata_);
        REQUIRE (ir_reader_mock.did_read_ir_data_);
    }
}
