#include "ProjectImportComponent.h"

const juce::String ProjectImportComponent::kProjectPickerDialogTitle = "Pick Project Directory";
const juce::String ProjectImportComponent::kIrPickerDialogTitle = "Pick Ir Directory";

ProjectImportComponent::ProjectImportComponent (
    const lager::reader<ProjectIrRepositoryModel> & model,
    lager::context<ProjectIrRepositoryAction> & context)
    : model_ (model)
    , project_paths_reader_ (
          model_.zoom (lager::lenses::attr (&ProjectIrRepositoryModel::project_paths)))
    , importing_state_reader_ (
          model_.zoom (lager::lenses::attr (&ProjectIrRepositoryModel::importing_project_ir_state)))
    , current_ir_reader_ (
          model_.zoom (lager::lenses::attr (&ProjectIrRepositoryModel::current_project_ir)))
    , context_ (context)

{
    addAndMakeVisible (current_project_paths_);
    addAndMakeVisible (current_ir_);
    addAndMakeVisible (add_project_path_button_);
    addAndMakeVisible (import_project_ir_button_);

    import_project_ir_button_.setEnabled (false);

    lager::watch (project_paths_reader_,
                  [&] (immer::flex_vector<std::filesystem::path> project_paths)
                  {
                      std::string path_list = "Project Paths: ";
                      for (auto & project_path : project_paths)
                          path_list += project_path.string () + ",";

                      current_project_paths_.setText (path_list, juce::dontSendNotification);
                      if (project_paths.size () > 0)
                          import_project_ir_button_.setEnabled (true);
                  });

    lager::watch (current_ir_reader_,
                  [&] (const CurrentProjectIrOptional & current_project_ir)
                  {
                      if (current_project_ir.has_value ())
                          current_ir_.setText ("Current Ir: " + current_project_ir.value (),
                                               juce::dontSendNotification);
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

    import_project_ir_button_.onClick = [&]
    {
        ir_picker_ = std::make_unique<juce::FileChooser> (kIrPickerDialogTitle);
        auto directory_flags =
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;
        ir_picker_->launchAsync (directory_flags,
                                 [&] (const juce::FileChooser & chooser)
                                 {
                                     std::filesystem::path ir_path =
                                         chooser.getResult ().getFullPathName ().toStdString ();
                                     auto identifier = ir_path.stem ();

                                     lager::watch (importing_state_reader_,
                                                   [&] (const ProjectIrLoadingState & state)
                                                   {
                                                       if (state == ProjectIrLoadingState::kSuccess)
                                                           context_.dispatch (LoadProjectIrAction {
                                                               .ir_identifier = identifier});
                                                   });

                                     context_.dispatch (ImportProjectIrAction {.import_project_ir {
                                         .ir_path = ir_path,
                                         .name = ir_path.stem (),
                                         .description = "None",
                                     }});
                                 });
    };
}

void ProjectImportComponent::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;

    layout.items.add (juce::FlexItem (current_project_paths_).withFlex (1.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (add_project_path_button_).withFlex (1.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (import_project_ir_button_).withFlex (1.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (current_ir_).withFlex (1.f));

    layout.performLayout (getLocalBounds ().toFloat ().reduced (LookAndFeel::kPadding));
}