#include "ProjectImportComponent.h"

const juce::String ProjectImportComponent::kProjectPickerDialogTitle = "PickProjectDirectory";

ProjectImportComponent::ProjectImportComponent (
    const lager::reader<ProjectIrRepositoryModel> & model,
    lager::context<ProjectIrRepositoryAction> & context)
    : model_ (model)
    , project_paths_reader_ (
          model_.zoom (lager::lenses::attr (&ProjectIrRepositoryModel::project_paths)))
    , context_ (context)
{
    addAndMakeVisible (current_project_paths_);
    addAndMakeVisible (add_project_path_button_);

    lager::watch (project_paths_reader_,
                  [&] (immer::flex_vector<std::filesystem::path> project_paths)
                  {
                      std::string path_list;
                      for (auto & project_path : project_paths)
                          path_list += project_path.string () + ",";

                      current_project_paths_.setText (path_list, juce::dontSendNotification);
                  });

    add_project_path_button_.onClick = [&]
    {
        directory_picker_ = std::make_unique<juce::FileChooser> (kProjectPickerDialogTitle);
        auto directory_flags =
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectDirectories;
        directory_picker_->launchAsync (
            directory_flags,
            [&] (const juce::FileChooser & chooser)
            {
                context_.dispatch (AddProjectPathAction {
                    .project_path = chooser.getResult ().getFullPathName ().toStdString ()});
            });
    };
}

void ProjectImportComponent::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;

    layout.items.add (juce::FlexItem (current_project_paths_).withFlex (1.f));
    layout.items.add (juce::FlexItem (add_project_path_button_).withFlex (1.f));

    layout.performLayout (getLocalBounds ().toFloat ().reduced (LookAndFeel::kPadding));
}

void ProjectImportComponent::paint (juce::Graphics & g)
{
    g.setColour (juce::Colours::green);
    g.fillAll ();
}