#pragma once
#include "../io/IrReader.h"
#include "../io/IrWriter.h"
#include "ProjectIrRepositoryModel.h"

#include <lager/reader.hpp>

class ProjectIrImportController
{
public:
    ProjectIrImportController (const lager::reader<ProjectIrRepositoryModel> & model,
                               IrReader & ir_reader,
                               IrWriter & ir_writer);

private:
    void TransferIrToProject (const std::filesystem::path & project_path,
                              const ImportProjectIr & import_project_ir);

    lager::reader<ProjectIrRepositoryModel> model_;
    lager::reader<ImportProjectIrOptional> import_project_ir_reader_;
    IrReader & ir_reader_;
    IrWriter & ir_writer_;
};
