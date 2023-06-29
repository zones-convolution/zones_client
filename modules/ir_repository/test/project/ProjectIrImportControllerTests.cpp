#include "ReaderWriterMocks.h"
#include "ir_repository/project/ProjectIrImportController.h"
#include "ir_repository/project/ProjectIrRepositoryAction.h"
#include "ir_repository/project/ProjectIrRepositoryModel.h"

#include <catch2/catch_test_macros.hpp>
#include <lager/context.hpp>
#include <lager/event_loop/manual.hpp>
#include <lager/state.hpp>

template <typename ActionType, typename TargetActionType>
void RequireDispatchedAt (size_t position, const std::vector<ActionType> & actions)
{
    const auto * action = std::get_if<TargetActionType> (&actions [position]);
    REQUIRE (action != nullptr);
}

template <typename ActionType>
class TrackingContext
{
public:
    lager::with_manual_event_loop loop {};
    std::vector<ActionType> dispatched_actions_;
    lager::context<ActionType> context_ = {[&] (const auto & action)
                                           {
                                               dispatched_actions_.push_back (action);
                                               return lager::future {};
                                           },
                                           loop,
                                           {}};
};

TEST_CASE ("imports project ir", "[ProjectIrImportController]")
{
    IrReaderMock ir_reader_mock_;
    IrWriterMock ir_writer_mock_;

    SECTION ("dispatches failure action when no valid project paths exist")
    {
        auto model_state = lager::make_state (ProjectIrRepositoryModel {}, lager::automatic_tag {});
        TrackingContext<ProjectIrRepositoryAction> tracking_context;

        ProjectIrImportController project_ir_import_controller {
            model_state, tracking_context.context_, ir_reader_mock_, ir_writer_mock_};

        auto [model, effect] =
            Update (model_state.get (),
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
        auto valid_dir =
            juce::File::getSpecialLocation (juce::File::SpecialLocationType::tempDirectory);

        auto model_state = lager::make_state (
            ProjectIrRepositoryModel {
                .project_paths = {valid_dir.getFullPathName ().toStdString ()}},
            lager::automatic_tag {});

        TrackingContext<ProjectIrRepositoryAction> tracking_context;
        ProjectIrImportController project_ir_import_controller {
            model_state, tracking_context.context_, ir_reader_mock_, ir_writer_mock_};

        auto [model, effect] =
            Update (model_state.get (),
                    ImportProjectIrAction {.import_project_ir = {.ir_path = "path/to/ir.wav",
                                                                 .name = "name",
                                                                 .description = "description"}});
        model_state.set (model);

        RequireDispatchedAt<ProjectIrRepositoryAction, ImportProjectIrLoadingAction> (
            0, tracking_context.dispatched_actions_);

        RequireDispatchedAt<ProjectIrRepositoryAction, ImportProjectIrSuccessAction> (
            1, tracking_context.dispatched_actions_);

        REQUIRE (ir_writer_mock_.did_write_ir_data_);
        REQUIRE (ir_writer_mock_.did_write_ir_metadata_);
        REQUIRE (ir_reader_mock_.did_read_ir_data_);
    }
}
