#include "HomeViewComponent.h"

#include "look_and_feel/LookAndFeel.h"

const juce::String HomeViewComponent::kPathPickerDialogTitle = "Pick Project Directory";

HomeViewComponent::HomeViewComponent (const lager::reader<Model> & model,
                                      lager::context<Action> & context)
    : context_ (context)
    , ir_loading_reader_ (model [&Model::ir_loading_model])
    , ir_reader_ (model [&Model::ir_loading_model][&IrLoadingModel::ir_path])
    , ir_repository_reader_ (model [&Model::ir_repository_model])
    , user_irs_reader_ (model [&Model::ir_repository_model][&IrRepositoryModel::user_irs])
{
    addAndMakeVisible (add_path_button_);

    add_path_button_.onClick = [&] { AddPath (); };
}

void HomeViewComponent::AddPath ()
{
    directory_picker_ = std::make_unique<juce::FileChooser> (kPathPickerDialogTitle);
    auto directory_flags =
        juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectDirectories;

    directory_picker_->launchAsync (
        directory_flags,
        [&] (const juce::FileChooser & chooser)
        {
            auto path_name = chooser.getResult ().getFullPathName ().toStdString ();
            current_paths_ =
                current_paths_.push_back (chooser.getResult ().getFullPathName ().toStdString ());
            context_.dispatch (RefreshUserIrsAction {.search_paths = current_paths_});
        });
}

void HomeViewComponent::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;

    layout.items.add (LookAndFeel::ButtonFlexItem (add_path_button_));
    layout.items.add (LookAndFeel::kFlexSpacer);

    layout.performLayout (getLocalBounds ().toFloat ());
}

void HomeViewComponent::Update (const HomeView & home_view)
{
}
