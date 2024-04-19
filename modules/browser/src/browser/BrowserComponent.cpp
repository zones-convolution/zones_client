#include "BrowserComponent.h"

#include "zones_look_and_feel/LookAndFeel.h"

const juce::String BrowserComponent::kPathPickerDialogTitle = "Pick Project Directory";

BrowserComponent::BrowserComponent (const lager::reader<Model> & model,
                                    lager::context<Action> & context)
    : context_ (context)
    , ir_loading_reader_ (model [&Model::ir_loading_model])
    , ir_reader_ (model [&Model::ir_loading_model][&IrLoadingModel::ir_path])
    , ir_repository_reader_ (model [&Model::ir_repository_model])
    , user_irs_reader_ (model [&Model::ir_repository_model][&IrRepositoryModel::user_irs])
{
    addAndMakeVisible (current_ir_label_);
    addAndMakeVisible (add_path_button_);
    addAndMakeVisible (ir_combo_box_);

    DisplayCurrentIr ();

    lager::watch (ir_reader_, [&] (const auto &) { DisplayCurrentIr (); });
    lager::watch (user_irs_reader_, [&] (const auto &) { UpdateIrList (); });

    add_path_button_.onClick = [&] { AddPath (); };
    ir_combo_box_.onChange = [&] { SelectIr (); };
}

void BrowserComponent::SelectIr () const
{
    auto user_irs = user_irs_reader_.get ();
    const auto & selected_ir = user_irs [ir_combo_box_.getSelectedItemIndex ()];
    context_.dispatch (LoadIrAction {.search_paths = current_paths_,
                                     .ir_path = *selected_ir.name,
                                     .target_format = TargetFormat::kStereo});
}

void BrowserComponent::DisplayCurrentIr ()
{
    auto ir = ir_reader_.get ();
    if (ir.has_value ())
    {
        current_ir_label_.setText ("Ir: " + ir->string (), juce::dontSendNotification);
        resized ();
    }
}

void BrowserComponent::UpdateIrList ()
{
    ir_combo_box_.clear (juce::NotificationType::dontSendNotification);
    auto user_irs = user_irs_reader_.get ();
    for (auto i = 0; i < user_irs.size (); ++i)
    {
        auto display_name = user_irs [i].name;
        auto channel_format = user_irs [i].channel_format;
        if (channel_format.has_value () && display_name.has_value ())
            ir_combo_box_.addItem (
                *display_name + " | " + IrMetadata::ChannelFormatToString (*channel_format), i + 1);
    }
}

void BrowserComponent::AddPath ()
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