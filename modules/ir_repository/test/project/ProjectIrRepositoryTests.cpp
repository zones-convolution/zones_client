#include "ir_repository/io/IrReader.h"
#include "ir_repository/io/IrWriter.h"
#include "ir_repository/project/ProjectIrPickerDelegate.h"
#include "ir_repository/project/ProjectIrRepository.h"
#include "ir_repository/project/ProjectIrRepositoryAction.h"
#include "ir_repository/project/ProjectIrRepositoryModel.h"
#include "ir_repository/project/ProjectPathPickerDelegate.h"

#include <catch2/catch_test_macros.hpp>
#include <lager/context.hpp>
#include <lager/state.hpp>

class IrPickerDelegateMock : public ProjectIrPickerDelegate
{
public:
    virtual ~IrPickerDelegateMock () = default;
    void PickIr (ProjectIrPickerCallback callback) override
    {
        did_pick_ir_ = true;
        callback (result_);
    }

    Result result_ {
        .name = "mock_name",
        .path = "/mock/path/ir.wav",
        .description = "mock_description",
    };

    bool did_pick_ir_ = false;
};

class ProjectPathPickerDelegateMock : public ProjectPathPickerDelegate
{
public:
    virtual ~ProjectPathPickerDelegateMock () = default;

    void PickPath (ProjectPathPickerCallback callback) override
    {
        did_pick_path_ = true;
        callback (result_);
    }

    Result result_ {
        .path = "/mock/path/project",
    };

    bool did_pick_path_ = false;
};

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

TEST_CASE ("loading new impulse responses to project", "[ProjectIrRepository]")
{
    IrPickerDelegateMock ir_picker_delegate_mock;
    ProjectPathPickerDelegateMock project_path_picker_delegate_mock;
    IrReaderMock ir_reader_mock_;
    IrWriterMock ir_writer_mock_;

    auto model_state = lager::make_state (ProjectIrRepositoryModel {});
    auto context = lager::context<ProjectIrRepositoryAction> ();

    SECTION ("loads correct identifier")
    {
        ProjectIrRepository project_ir_repository {model_state,
                                                   context,
                                                   ir_picker_delegate_mock,
                                                   project_path_picker_delegate_mock,
                                                   ir_reader_mock_,
                                                   ir_writer_mock_};

        REQUIRE (ir_writer_mock_.did_write_ir_data_);
        REQUIRE (ir_writer_mock_.did_write_ir_metadata_);

        project_ir_repository.LoadNewProjectIr (
            [&] (std::string ir_identifier)
            { REQUIRE (ir_identifier == ir_picker_delegate_mock.result_.name); });
    }
}
