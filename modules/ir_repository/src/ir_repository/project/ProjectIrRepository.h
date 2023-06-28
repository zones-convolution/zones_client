#pragma once

#include "../IrData.h"
#include "../IrMetadata.h"
#include "../io/IrReader.h"
#include "../io/IrWriter.h"
#include "ProjectIrRepositoryAction.h"
#include "ProjectIrRepositoryModel.h"
#include "ProjectPathPickerDelegate.h"

#include <filesystem>
#include <functional>
#include <juce_core/juce_core.h>
#include <lager/context.hpp>
#include <lager/reader.hpp>

class ProjectIrRepository
{
public:
    using LoadNewProjectIrCallback = std::function<void (const std::string &)>;

    ProjectIrRepository (lager::reader<ProjectIrRepositoryModel> reader,
                         lager::context<ProjectIrRepositoryAction> context,
                         ProjectPathPickerDelegate & project_path_picker_delegate,
                         IrReader & ir_reader,
                         IrWriter & ir_writer);

    void LoadNewProjectIr (const std::filesystem::path & ir_path,
                           const std::string & name,
                           const std::string & description,
                           LoadNewProjectIrCallback ir_result);

    [[nodiscard]] IrMetadata LoadIrMetaData (const std::string & ir_identifier);
    void LoadIrData (const std::string & ir_identifier, IrData & ir_data);

private:
    void TransferIrToProject (std::filesystem::path original_path,
                              std::filesystem::path project_path,
                              std::string name,
                              std::string description);

    lager::reader<ProjectIrRepositoryModel> reader_;
    ProjectPathPickerDelegate & project_path_picker_delegate_;
    IrReader & ir_reader_;
    IrWriter & ir_writer_;
};