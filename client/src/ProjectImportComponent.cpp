#include "ProjectImportComponent.h"

const juce::String ProjectImportComponent::kProjectPickerDialogTitle = "Pick Project Directory";
const juce::String ProjectImportComponent::kIrPickerDialogTitle = "Pick Ir Directory";

ProjectImportComponent::ProjectImportComponent (
    const lager::reader<ProjectIrRepositoryModel> & project_ir_model,
    lager::context<ProjectIrRepositoryAction> & project_ir_context,
    lager::context<ParameterAction> & parameter_context)
    : project_ir_model_ (project_ir_model)
    , project_ir_context_ (project_ir_context)
    , parameter_context_ (parameter_context)
    , project_paths_reader_ (
          project_ir_model_.zoom (lager::lenses::attr (&ProjectIrRepositoryModel::project_paths)))
    , importing_state_reader_ (project_ir_model_.zoom (
          lager::lenses::attr (&ProjectIrRepositoryModel::importing_project_ir_state)))
    , current_ir_reader_ (project_ir_model_.zoom (
          lager::lenses::attr (&ProjectIrRepositoryModel::current_project_ir)))

{
    room_size_slider_.setRange ({.2f, 2.f}, 0);

    addAndMakeVisible (current_project_paths_);
    addAndMakeVisible (current_ir_);
    addAndMakeVisible (add_project_path_button_);
    addAndMakeVisible (import_project_ir_button_);
    addAndMakeVisible (project_ir_combo_box_);
    addAndMakeVisible (room_size_slider_);
    addAndMakeVisible (room_size_label_);
    addAndMakeVisible (dry_wet_mix_slider_);
    addAndMakeVisible (dry_wet_label_);

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

    add_project_path_button_.onClick = [&] { AddProjectPath (); };
    import_project_ir_button_.onClick = [&] { ImportProjectIr (); };
    project_ir_combo_box_.onChange = [&] { SelectProjectIr (project_ir_context); };

    room_size_label_.setText ("Room Size", juce::dontSendNotification);
    room_size_slider_.setRange ({0.1f, 2.f}, 0);
    room_size_slider_.setPopupDisplayEnabled (true, true, getTopLevelComponent ());
    room_size_slider_.onDragEnd = [&]
    {
        parameter_context.dispatch (
            SetRoomSizeAction {.room_size = (float) room_size_slider_.getValue ()});
    };

    dry_wet_label_.setText ("Dry/Wet", juce::dontSendNotification);
    dry_wet_mix_slider_.setRange ({0.f, 1.f}, 0);
    dry_wet_mix_slider_.setPopupDisplayEnabled (true, true, getTopLevelComponent ());
    dry_wet_mix_slider_.onValueChange = [&]
    {
        parameter_context.dispatch (
            SetDryWetMixAction {.dry_wet_mix = (float) dry_wet_mix_slider_.getValue ()});
    };
}

void ProjectImportComponent::SelectProjectIr (
    const lager::context<ProjectIrRepositoryAction> & context) const
{
    auto & [identifier, _] = project_data_ [project_ir_combo_box_.getSelectedItemIndex ()];
    context.dispatch (LoadProjectIrAction {.ir_identifier = identifier});
}

void ProjectImportComponent::ImportProjectIr ()
{
    ir_picker_ = std::__1::make_unique<juce::FileChooser> (kIrPickerDialogTitle);
    auto directory_flags =
        juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;
    ir_picker_->launchAsync (
        directory_flags,
        [&] (const juce::FileChooser & chooser)
        {
            std::filesystem::path ir_path = chooser.getResult ().getFullPathName ().toStdString ();
            auto identifier = ir_path.stem ();

            lager::watch (importing_state_reader_,
                          [&] (const ProjectIrLoadingState & state)
                          {
                              if (state == ProjectIrLoadingState::kSuccess)
                                  project_ir_context_.dispatch (
                                      LoadProjectIrAction {.ir_identifier = identifier});
                          });

            project_ir_context_.dispatch (ImportProjectIrAction {.import_project_ir {
                .ir_path = ir_path,
                .name = ir_path.stem (),
                .description = "None",
            }});
        });
}

void ProjectImportComponent::AddProjectPath ()
{
    directory_picker_ = std::__1::make_unique<juce::FileChooser> (kProjectPickerDialogTitle);
    auto directory_flags =
        juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectDirectories;
    directory_picker_->launchAsync (
        directory_flags,
        [&] (const juce::FileChooser & chooser)
        {
            project_ir_context_.dispatch (AddProjectPathAction {
                .project_path = chooser.getResult ().getFullPathName ().toStdString ()});
        });
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
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (room_size_label_).withFlex (1.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (room_size_slider_).withFlex (1.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (dry_wet_label_).withFlex (1.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (dry_wet_mix_slider_).withFlex (1.f));

    layout.performLayout (getLocalBounds ().toFloat ().reduced (LookAndFeel::kPadding));
}

void ProjectImportComponent::UpdateIrList ()
{
    IrReader ir_reader;
    project_ir_combo_box_.clear (juce::NotificationType::dontSendNotification);

    auto load_path = ProjectIrPaths (project_ir_model_).GetAvailableProjectPath ();
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
    if (! project_paths.empty ())
        import_project_ir_button_.setEnabled (true);
}

void ProjectImportComponent::DisplayCurrentIr ()
{
    auto current_project_ir = current_ir_reader_.get ();
    if (current_project_ir.has_value ())
        current_ir_.setText ("Current Ir: " + current_project_ir.value (),
                             juce::dontSendNotification);
}
