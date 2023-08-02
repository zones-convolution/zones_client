#pragma once

#include "../io/IrReader.h"
#include "ProjectIrRepositoryAction.h"
#include "ProjectIrRepositoryModel.h"

#include <lager/context.hpp>
#include <lager/reader.hpp>

class ProjectIrLoadController
{
public:
    ProjectIrLoadController (const lager::reader<ProjectIrRepositoryModel> & project_ir_reader,
                             lager::context<ProjectIrRepositoryAction> context,
                             IrReader & ir_reader);

    const IrData * GetCurrentProjectIrData () const;
    const IrMetadata & GetCurrentProjectIrMetadata () const;

private:
    void LoadProjectIr (const std::filesystem::path & project_path,
                        const std::string & ir_identifier);

    lager::reader<ProjectIrRepositoryModel> project_ir_reader_;
    lager::context<ProjectIrRepositoryAction> context_;
    lager::reader<CurrentProjectIrOptional> current_project_ir_reader_;
    IrReader & ir_reader_;
    IrData current_project_ir_data_;
    IrMetadata current_project_ir_metadata_;
};
