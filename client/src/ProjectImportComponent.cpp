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
    addAndMakeVisible (project_ir_combo_box_);

    import_project_ir_button_.setEnabled (false);

    UpdateIrList ();
    DisplayProjectPaths ();
    DisplayCurrentIr ();

    lager::watch (project_paths_reader_,
                  [&] (const auto &)
                  {
                      DisplayProjectPaths ();
                      UpdateIrList ();
                  });

    lager::watch (current_ir_reader_,
                  [&] (const auto &)
                  {
                      DisplayCurrentIr ();
                      UpdateIrList ();
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

    project_ir_combo_box_.onChange = [&]
    {
        auto & [identifier, _] = project_data_ [project_ir_combo_box_.getSelectedItemIndex ()];
        context.dispatch (LoadProjectIrAction {.ir_identifier = identifier});
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
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (project_ir_combo_box_).withFlex (1.f));

    layout.performLayout (getLocalBounds ().toFloat ().reduced (LookAndFeel::kPadding));
}

void ProjectImportComponent::UpdateIrList ()
{
    IrReader ir_reader;
    project_ir_combo_box_.clear (juce::NotificationType::dontSendNotification);

    auto load_path = ProjectIrPaths (model_).GetAvailableProjectPath ();
    if (! load_path.has_value ())
        return;
    auto ir_list = ir_reader.GetIrsInPath (load_path.value ());
    project_data_ = ir_list;

    for (auto item_number = 0; item_number < ir_list.size (); ++item_number)
    {
        auto identifier = ir_list [item_number].first;
        project_ir_combo_box_.addItem (identifier, item_number + 1);
    }
}

void ProjectImportComponent::DisplayProjectPaths ()
{
    auto project_paths = project_paths_reader_.get ();
    std::string path_list = "Project Paths: ";
    for (auto & project_path : project_paths)
        path_list += project_path.string () + ",";

    current_project_paths_.setText (path_list, juce::dontSendNotification);
    if (project_paths.size () > 0)
        import_project_ir_button_.setEnabled (true);
}

void ProjectImportComponent::DisplayCurrentIr ()
{
    auto current_project_ir = current_ir_reader_.get ();
    if (current_project_ir.has_value ())
        current_ir_.setText ("Current Ir: " + current_project_ir.value (),
                             juce::dontSendNotification);
}
