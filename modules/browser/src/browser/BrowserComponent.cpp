#include "BrowserComponent.h"

#include "zones_look_and_feel/LookAndFeel.h"

const juce::String BrowserComponent::kPathPickerDialogTitle = "Pick Project Directory";

BrowserComponent::BrowserComponent (const lager::reader<IrLoadingModel> & ir_loading_reader,
                                    lager::context<IrLoadingAction> & ir_loading_context)
    : ir_loading_reader_ (ir_loading_reader)
    , ir_loading_context_ (ir_loading_context)
    , ir_reader_ (ir_loading_reader [&IrLoadingModel::ir_path])
{
    addAndMakeVisible (current_ir_label_);
    addAndMakeVisible (add_path_button_);
    addAndMakeVisible (ir_combo_box_);

    DisplayCurrentIr ();

    lager::watch (ir_reader_,
                  [&] (const auto &)
                  {
                      DisplayCurrentIr ();
                      UpdateIrList ();
                  });

    add_path_button_.onClick = [&] { AddPath (); };
    ir_combo_box_.onChange = [&] { SelectIr (); };
}

void BrowserComponent::SelectIr () const
{
    ir_loading_context_.dispatch (LoadIrAction {
        .search_paths = current_paths_, .ir_path = ir_combo_box_.getText ().toStdString ()});
}

void BrowserComponent::DisplayCurrentIr ()
{
    auto ir = ir_reader_.get ();
    if (ir.has_value ())
        current_ir_label_.setText ("Ir: " + ir.value ().string (), juce::dontSendNotification);
}

void BrowserComponent::UpdateIrList ()
{
    IrReader ir_reader;
    ir_combo_box_.clear (juce::NotificationType::dontSendNotification);

    std::vector<std::string> ir_paths;
    for (auto & path : current_paths_)
    {
        auto search_directory = juce::File (path.string ());
        auto ir_files =
            search_directory.findChildFiles (juce::File::TypesOfFileToFind::findDirectories, false);

        for (const auto & ir_file : ir_files)
        {
            auto ir_path =
                std::filesystem::path (ir_file.getFullPathName ().toStdString ()).stem ();
            ir_paths.push_back (ir_path);
        }
    }

    for (auto i = 0; i < ir_paths.size (); ++i)
        ir_combo_box_.addItem (ir_paths [i], i + 1);
}

void BrowserComponent::AddPath ()
{
    directory_picker_ = std::make_unique<juce::FileChooser> (kPathPickerDialogTitle);
    auto directory_flags =
        juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectDirectories;

    directory_picker_->launchAsync (directory_flags,
                                    [&] (const juce::FileChooser & chooser)
                                    {
                                        auto path_name =
                                            chooser.getResult ().getFullPathName ().toStdString ();
                                        current_paths_ = current_paths_.push_back (
                                            chooser.getResult ().getFullPathName ().toStdString ());
                                        UpdateIrList ();
                                    });
}

void BrowserComponent::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;

    layout.items.add (LookAndFeel::ButtonFlexItem (add_path_button_));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (LookAndFeel::LabelFlexItem (current_ir_label_));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (LookAndFeel::ComboFlexItem (ir_combo_box_));

    layout.performLayout (getLocalBounds ().toFloat ());
}