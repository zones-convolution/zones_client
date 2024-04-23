#pragma once
#include "../BrowserModel.h"
#include "model/Action.h"
#include "model/Model.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <lager/context.hpp>
#include <lager/reader.hpp>

class HomeViewComponent : public juce::Component
{
public:
    HomeViewComponent (const lager::reader<Model> & model, lager::context<Action> & context);
    void Update (const HomeView & home_view);
    void resized () override;

    void AddPath ();

private:
    static const juce::String kPathPickerDialogTitle;

    lager::context<Action> context_;

    lager::reader<IrLoadingModel> ir_loading_reader_;
    lager::reader<std::optional<std::filesystem::path>> ir_reader_;

    lager::reader<IrRepositoryModel> ir_repository_reader_;
    lager::reader<immer::flex_vector<IrMetadata>> user_irs_reader_;

    juce::TextButton add_path_button_ {"Add Path"};
    immer::flex_vector<std::filesystem::path> current_paths_;

    std::unique_ptr<juce::FileChooser> directory_picker_;
};
