#pragma once
#include "../io/IrReader.h"
#include "../io/IrWriter.h"
#include "ProjectIrRepositoryAction.h"
#include "ProjectIrRepositoryModel.h"

#include <lager/context.hpp>
#include <lager/reader.hpp>

class ProjectIrImportController
{
public:
    ProjectIrImportController (const lager::reader<ProjectIrRepositoryModel> & model,
                               lager::context<ProjectIrRepositoryAction> & context,
                               IrReader & ir_reader,
                               IrWriter & ir_writer);

private:
    void TransferIrToProject (const std::filesystem::path & project_path,
                              const ImportProjectIr & import_project_ir);

    lager::reader<ProjectIrRepositoryModel> model_;
    lager::context<ProjectIrRepositoryAction> context_;
    lager::reader<ImportProjectIrOptional> import_project_ir_reader_;
    IrReader & ir_reader_;
    IrWriter & ir_writer_;
};
