#include "ir_repository/io/IrReader.h"
#include "ir_repository/io/IrWriter.h"
#include "ir_repository/project/ProjectIrImportController.h"
#include "ir_repository/project/ProjectIrRepositoryAction.h"
#include "ir_repository/project/ProjectIrRepositoryModel.h"

#include <catch2/catch_test_macros.hpp>
#include <lager/context.hpp>
#include <lager/event_loop/manual.hpp>
#include <lager/state.hpp>

class IrReaderMock : public IrReader
{
public:
    virtual ~IrReaderMock () = default;

    void ReadIrData (const std::filesystem::path & load_path,
                     const std::string & ir_identifier,
                     IrData & ir_data) override
    {
    }

    IrMetadata ReadIrMetadata (const std::filesystem::path & load_path,
                               const std::string & ir_identifier) override
    {
        return ir_metadata_;
    }

    IrMetadata ir_metadata_;
};

class IrWriterMock : public IrWriter
{
public:
    virtual ~IrWriterMock () = default;

    void WriteIrMetadata (const std::filesystem::path & write_path,
                          const std::string & ir_identifier,
                          const IrMetadata & ir_metadata) override
    {
        did_write_ir_metadata_ = true;
    }

    void WriteIrData (const std::filesystem::path & write_path,
                      const std::string & ir_identifier,
                      const IrData & ir_data) override
    {
        did_write_ir_data_ = true;
    }

    bool did_write_ir_data_ = false;
    bool did_write_ir_metadata_ = false;
};

TEST_CASE ("imports project ir", "[ProjectIrImportController]")
{
    IrReaderMock ir_reader_mock_;
    IrWriterMock ir_writer_mock_;
    auto loop = lager::with_manual_event_loop {};

    SECTION ("loads project ir")
    {
        auto model_state = lager::make_state (ProjectIrRepositoryModel {});

        auto context = lager::context<ProjectIrRepositoryAction> {
            [&] (const auto & action)
            {
                const auto * loading_action = std::get_if<ImportProjectIrLoadingAction> (&action);
                REQUIRE (loading_action != nullptr);
                return lager::future {};
            },
            loop,
            {}};

        ProjectIrImportController project_ir_import_controller {
            model_state, context, ir_reader_mock_, ir_writer_mock_};
    }
}
