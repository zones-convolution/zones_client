#pragma once

#include "ir_repository/io/IrReader.h"
#include "ir_repository/project/ProjectIrPaths.h"
#include "ir_repository/project/ProjectIrRepositoryAction.h"
#include "ir_repository/project/ProjectIrRepositoryModel.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <lager/reader.hpp>

class BrowserComponent : public juce::Component
{
public:
    BrowserComponent (const lager::reader<ProjectIrRepositoryModel> & project_ir_reader,
                      lager::context<ProjectIrRepositoryAction> & project_ir_context);

private:
    static const juce::String kProjectPickerDialogTitle;
    static const juce::String kIrPickerDialogTitle;

    void DisplayProjectPaths ();
    void DisplayCurrentIr ();
    void UpdateIrList ();

    void AddProjectPath ();
    void ImportProjectIr ();
    void SelectProjectIr () const;

    void resized () override;

    juce::TextButton add_project_path_button_ {"Add Project Path"};
    juce::TextButton import_project_ir_button_ {"Import Project Ir"};

    juce::ComboBox project_ir_combo_box_ {};

    juce::Label current_project_paths_;
    juce::Label current_ir_;

    lager::reader<ProjectIrRepositoryModel> project_ir_reader_;
    lager::context<ProjectIrRepositoryAction> project_ir_context_;
    lager::reader<immer::flex_vector<std::filesystem::path>> project_paths_reader_;
    lager::reader<CurrentProjectIrOptional> current_ir_reader_;
    lager::reader<ProjectIrLoadingState> importing_state_reader_;

    IrReader::ProjectData project_data_;
    std::unique_ptr<juce::FileChooser> directory_picker_;
    std::unique_ptr<juce::FileChooser> ir_picker_;
};
