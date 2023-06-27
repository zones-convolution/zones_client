#pragma once

#include "../IrData.h"
#include "../IrMetadata.h"
#include "../io/ScopedIrIO.h"
#include "ProjectIrRepositoryAction.h"
#include "ProjectIrRepositoryModel.h"

#include <filesystem>
#include <lager/context.hpp>
#include <lager/reader.hpp>

class ProjectIrRepository
{
public:
    ProjectIrRepository (lager::reader<ProjectIrRepositoryModel> reader,
                         lager::context<ProjectIrRepositoryAction> context);

    void LinkProjectPath (const std::filesystem::path & project_path);
    std::filesystem::path GetProjectPath ();

    [[nodiscard]] IrMetadata LoadIrMetaData (const std::string & ir_identifier);
    void LoadIrData (const std::string & ir_identifier, IrData & ir_data);

private:
    static bool IsPathValidDirectory (const std::filesystem::path & path);
};