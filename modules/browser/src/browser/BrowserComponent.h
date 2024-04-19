#pragma once

#include "model/Action.h"
#include "model/Model.h"

#include <immer/flex_vector.hpp>
#include <juce_gui_basics/juce_gui_basics.h>
#include <lager/reader.hpp>

class BrowserComponent : public juce::Component
{
public:
    BrowserComponent (const lager::reader<Model> & model, lager::context<Action> & context);

private:
    static const juce::String kPathPickerDialogTitle;

    void DisplayCurrentIr ();
    void UpdateIrList ();

    void AddPath ();
    void SelectIr () const;

    void resized () override;

    lager::context<Action> context_;

    lager::reader<IrLoadingModel> ir_loading_reader_;
    lager::reader<std::optional<std::filesystem::path>> ir_reader_;

    lager::reader<IrRepositoryModel> ir_repository_reader_;
    lager::reader<immer::flex_vector<IrMetadata>> user_irs_reader_;

    juce::TextButton add_path_button_ {"Add Path"};
    immer::flex_vector<std::filesystem::path> current_paths_;

    juce::ComboBox ir_combo_box_ {};
    juce::Label current_ir_label_;

    std::unique_ptr<juce::FileChooser> directory_picker_;
    std::unique_ptr<juce::FileChooser> ir_picker_;
};
