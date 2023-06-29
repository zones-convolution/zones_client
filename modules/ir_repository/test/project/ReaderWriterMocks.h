#pragma once

#include "ir_repository/io/IrReader.h"
#include "ir_repository/io/IrWriter.h"

class IrReaderMock : public IrReader
{
public:
    virtual ~IrReaderMock () = default;

    void ReadIrData (const std::filesystem::path & load_path,
                     const std::string & ir_identifier,
                     IrData & ir_data) override
    {
        did_read_ir_data_ = true;
    }

    IrMetadata ReadIrMetadata (const std::filesystem::path & load_path,
                               const std::string & ir_identifier) override
    {
        did_read_ir_metadata_ = true;
        return ir_metadata_;
    }

    IrMetadata ir_metadata_;
    bool did_read_ir_data_ = false;
    bool did_read_ir_metadata_ = false;
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