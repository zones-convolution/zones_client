#pragma once

#include "../IrData.h"
#include "../IrMetadata.h"
#include "../io/ScopedIrIO.h"
#include "ProjectIrPickerDelegate.h"
#include "ProjectIrRepositoryAction.h"
#include "ProjectIrRepositoryModel.h"

#include <filesystem>
#include <functional>
#include <lager/context.hpp>
#include <lager/reader.hpp>

class ProjectIrRepository
{
public:
    using LoadNewProjectIrCallback = std::function<void (const std::string &)>;

    ProjectIrRepository (lager::reader<ProjectIrRepositoryModel> reader,
                         lager::context<ProjectIrRepositoryAction> context,
                         ProjectIrPickerDelegate & project_ir_picker_delegate);

    void LinkProjectPath (const std::filesystem::path & project_path);
    std::optional<std::filesystem::path> GetProjectPath ();

    void LoadNewProjectIr (LoadNewProjectIrCallback callback);

    [[nodiscard]] IrMetadata LoadIrMetaData (const std::string & ir_identifier);
    void LoadIrData (const std::string & ir_identifier, IrData & ir_data);

private:
    static bool IsPathValidDirectory (const std::filesystem::path & path);
    lager::reader<ProjectIrRepositoryModel> reader_;
    ProjectIrPickerDelegate & project_ir_picker_delegate_;
};