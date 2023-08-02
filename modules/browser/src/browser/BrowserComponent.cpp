#include "BrowserComponent.h"

#include "zones_look_and_feel/LookAndFeel.h"

const juce::String BrowserComponent::kProjectPickerDialogTitle = "Pick Project Directory";
const juce::String BrowserComponent::kIrPickerDialogTitle = "Pick Ir Directory";

BrowserComponent::BrowserComponent (
    const lager::reader<ProjectIrRepositoryModel> & project_ir_reader,
    lager::context<ProjectIrRepositoryAction> & project_ir_context)
    : project_ir_reader_ (project_ir_reader)
    , project_ir_context_ (project_ir_context)
    , project_paths_reader_ (
          project_ir_reader_.zoom (lager::lenses::attr (&ProjectIrRepositoryModel::project_paths)))
    , current_ir_reader_ (project_ir_reader_.zoom (
          lager::lenses::attr (&ProjectIrRepositoryModel::current_project_ir)))
    , importing_state_reader_ (project_ir_reader_.zoom (
          lager::lenses::attr (&ProjectIrRepositoryModel::importing_project_ir_state)))
{
    addAndMakeVisible (current_project_paths_);
    addAndMakeVisible (current_ir_);
    addAndMakeVisible (add_project_path_button_);
    addAndMakeVisible (import_project_ir_button_);
    addAndMakeVisible (project_ir_combo_box_);

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

    import_project_ir_button_.setEnabled (false);
    add_project_path_button_.onClick = [&] { AddProjectPath (); };
    import_project_ir_button_.onClick = [&] { ImportProjectIr (); };
    project_ir_combo_box_.onChange = [&] { SelectProjectIr (); };
}

void BrowserComponent::SelectProjectIr () const
{
    auto & [identifier, _] = project_data_ [project_ir_combo_box_.getSelectedItemIndex ()];
    project_ir_context_.dispatch (LoadProjectIrAction {.ir_identifier = identifier});
}

void BrowserComponent::DisplayProjectPaths ()
{
    auto project_paths = project_paths_reader_.get ();
    std::string path_list = "Project Paths: ";
    for (auto & project_path : project_paths)
        path_list += project_path.string () + ",";

    current_project_paths_.setText (path_list, juce::dontSendNotification);
    if (! project_paths.empty ())
        import_project_ir_button_.setEnabled (true);
}

void BrowserComponent::DisplayCurrentIr ()
{
    auto current_project_ir = current_ir_reader_.get ();
    if (current_project_ir.has_value ())
        current_ir_.setText ("Current Ir: " + current_project_ir.value (),
                             juce::dontSendNotification);
}

void BrowserComponent::UpdateIrList ()
{
    IrReader ir_reader;
    project_ir_combo_box_.clear (juce::NotificationType::dontSendNotification);

    auto load_path = ProjectIrPaths (project_ir_reader_).GetAvailableProjectPath ();
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

void BrowserComponent::ImportProjectIr ()
{
    ir_picker_ = std::make_unique<juce::FileChooser> (kIrPickerDialogTitle);
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

void BrowserComponent::AddProjectPath ()
{
    directory_picker_ = std::make_unique<juce::FileChooser> (kProjectPickerDialogTitle);
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

void BrowserComponent::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;

    layout.items.add (LookAndFeel::LabelFlexItem (current_project_paths_));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (LookAndFeel::ButtonFlexItem (add_project_path_button_));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (LookAndFeel::ButtonFlexItem (import_project_ir_button_));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (LookAndFeel::LabelFlexItem (current_ir_));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (LookAndFeel::ComboFlexItem (project_ir_combo_box_));

    layout.performLayout (getLocalBounds ().toFloat ());
}