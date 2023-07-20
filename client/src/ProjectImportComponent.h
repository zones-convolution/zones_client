#pragma once

#include "ir_repository/io/IrReader.h"
#include "ir_repository/project/ProjectIrPaths.h"
#include "ir_repository/project/ProjectIrRepositoryAction.h"
#include "ir_repository/project/ProjectIrRepositoryModel.h"
#include "zones_look_and_feel/LookAndFeel.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <lager/context.hpp>
#include <lager/reader.hpp>

class ProjectImportComponent : public juce::Component
{
public:
    ProjectImportComponent (const lager::reader<ProjectIrRepositoryModel> & model,
                            lager::context<ProjectIrRepositoryAction> & context);

    void resized () override;

private:
    void UpdateIrList ();
    void DisplayProjectPaths ();
    void DisplayCurrentIr ();

    juce::TextButton add_project_path_button_ {"Add Project Path"};
    juce::TextButton import_project_ir_button_ {"Import Project Ir"};

    juce::ComboBox project_ir_combo_box_ {};
    IrReader::ProjectData project_data_;

    juce::Label current_project_paths_;
    juce::Label current_ir_;
    static const juce::String kProjectPickerDialogTitle;
    static const juce::String kIrPickerDialogTitle;

    std::unique_ptr<juce::FileChooser> directory_picker_;
    std::unique_ptr<juce::FileChooser> ir_picker_;

    lager::reader<ProjectIrRepositoryModel> model_;
    lager::reader<immer::flex_vector<std::filesystem::path>> project_paths_reader_;
    lager::reader<ProjectIrLoadingState> importing_state_reader_;
    lager::reader<CurrentProjectIrOptional> current_ir_reader_;
    lager::context<ProjectIrRepositoryAction> context_;
    void AddProjectPath ();
    void ImportProjectIr ();
    void SelectProjectIr (const lager::context<ProjectIrRepositoryAction> & context) const;

    juce::Slider room_size_slider_ {juce::Slider::SliderStyle::LinearHorizontal,
                                    juce::Slider::TextEntryBoxPosition::NoTextBox};
};