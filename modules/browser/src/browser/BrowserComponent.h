#pragma once

#include "ir_format/io/IrReader.h"
#include "ir_repository/project/IrLoadingAction.h"
#include "ir_repository/project/IrLoadingModel.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <lager/reader.hpp>

class BrowserComponent : public juce::Component
{
public:
    BrowserComponent (const lager::reader<IrLoadingModel> & ir_loading_reader,
                      lager::context<IrLoadingAction> & ir_loading_context);

private:
    static const juce::String kPathPickerDialogTitle;

    void DisplayCurrentIr ();
    void UpdateIrList ();

    void AddPath ();
    void SelectIr () const;

    void resized () override;

    lager::reader<IrLoadingModel> ir_loading_reader_;
    lager::context<IrLoadingAction> ir_loading_context_;
    lager::reader<std::optional<std::filesystem::path>> ir_reader_;

    juce::TextButton add_path_button_ {"Add Path"};
    immer::flex_vector<std::filesystem::path> current_paths_;

    juce::ComboBox ir_combo_box_ {};
    juce::Label current_ir_label_;

    std::unique_ptr<juce::FileChooser> directory_picker_;
    std::unique_ptr<juce::FileChooser> ir_picker_;
};
