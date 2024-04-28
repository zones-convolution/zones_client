#include "ImportComponent.h"

#include "look_and_feel/LookAndFeel.h"
#include "preferences/Preferences.h"

const juce::String SpeakerPositionComponent::kChoosePickerDialogTitle = "Pick Audio File";

const std::map<std::string, ChannelFormat> ImportComponent::kMicrophonePickerOptions = {
    {"Mono", ChannelFormat::kMono},
    {"Stereo", ChannelFormat::kStereo},
    {"FOA", ChannelFormat::kFoa}};

SpeakerPositionComponent::SpeakerPositionComponent ()
{
    addAndMakeVisible (centre_label_);
    addAndMakeVisible (left_label_);
    addAndMakeVisible (right_label_);

    addAndMakeVisible (centre_path_label_);
    addAndMakeVisible (left_path_label_);
    addAndMakeVisible (right_path_label_);

    centre_import_button_.onClick = [&] { AddPath (SpeakerPosition::kC); };
    left_import_button_.onClick = [&] { AddPath (SpeakerPosition::kL); };
    right_import_button_.onClick = [&] { AddPath (SpeakerPosition::kR); };

    addAndMakeVisible (centre_import_button_);
    addAndMakeVisible (left_import_button_);
    addAndMakeVisible (right_import_button_);
}

void SpeakerPositionComponent::AddPath (SpeakerPosition speaker_position)
{
    file_picker_ = std::make_unique<juce::FileChooser> (
        kChoosePickerDialogTitle, juce::File (), "*.wav,*.aiff,*.mp3");
    auto picker_flags =
        juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;
    file_picker_->launchAsync (picker_flags,
                               [&, speaker_position] (const juce::FileChooser & chooser)
                               {
                                   auto path = chooser.getResult ();
                                   if (path.exists ())
                                   {
                                       auto absolute_path = path.getFullPathName ().toStdString ();
                                       switch (speaker_position)
                                       {
                                           case SpeakerPosition::kC:
                                               position_map_.centre = absolute_path;
                                               break;
                                           case SpeakerPosition::kL:
                                               position_map_.left = absolute_path;
                                               break;
                                           case SpeakerPosition::kR:
                                               position_map_.right = absolute_path;
                                               break;
                                       }

                                       UpdateDisplay ();
                                   }
                               });
}

static juce::FlexBox CreateSpeakerPositionLayout (juce::Label & label,
                                                  juce::Label & path_label,
                                                  juce::Component & button)
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::row;
    layout.alignItems = juce::FlexBox::AlignItems::center;

    layout.items.add (LookAndFeel::LabelFlexItem (label));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (LookAndFeel::LabelFlexItem (path_label));
    layout.items.add (juce::FlexItem ().withFlex (1.0f));
    layout.items.add (LookAndFeel::ButtonFlexItem (button).withWidth (80.0f));

    return layout;
}

void SpeakerPositionComponent::resized ()
{
    auto centre_layout =
        CreateSpeakerPositionLayout (centre_label_, centre_path_label_, centre_import_button_);
    auto left_layout =
        CreateSpeakerPositionLayout (left_label_, left_path_label_, left_import_button_);
    auto right_layout =
        CreateSpeakerPositionLayout (right_label_, right_path_label_, right_import_button_);

    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;

    layout.items.add (juce::FlexItem (centre_layout).withHeight (LookAndFeel::kButtonHeight));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (left_layout).withHeight (LookAndFeel::kButtonHeight));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (right_layout).withHeight (LookAndFeel::kButtonHeight));

    layout.performLayout (getLocalBounds ());
}

void SpeakerPositionComponent::UpdateVisiblePositions (DisplayType display_type)
{
    auto update_position_enabled = [] (juce::Label & label, juce::Button & button, bool enabled)
    {
        label.setEnabled (enabled);
        button.setEnabled (enabled);
    };

    switch (display_type)
    {
        case DisplayType::kC:
            update_position_enabled (centre_label_, centre_import_button_, true);
            update_position_enabled (left_label_, left_import_button_, false);
            update_position_enabled (right_label_, right_import_button_, false);

            position_map_.left = std::nullopt;
            position_map_.right = std::nullopt;
            break;
        case DisplayType::kLR:
            update_position_enabled (centre_label_, centre_import_button_, false);
            update_position_enabled (left_label_, left_import_button_, true);
            update_position_enabled (right_label_, right_import_button_, true);

            position_map_.centre = std::nullopt;
            break;
        case DisplayType::kCLR:
            update_position_enabled (centre_label_, centre_import_button_, true);
            update_position_enabled (left_label_, left_import_button_, true);
            update_position_enabled (right_label_, right_import_button_, true);
            break;
    }

    UpdateDisplay ();
}

void SpeakerPositionComponent::UpdateDisplay ()
{
    auto update_path_label = [] (const std::optional<std::string> path, juce::Label & label)
    { label.setText (path.has_value () ? path.value () : "", juce::dontSendNotification); };

    update_path_label (position_map_.centre, centre_path_label_);
    update_path_label (position_map_.left, left_path_label_);
    update_path_label (position_map_.right, right_path_label_);

    resized ();
}

const PositionMap & SpeakerPositionComponent::GetPositionMap () const
{
    return position_map_;
}

ImportComponent::ImportComponent ()
{
    addAndMakeVisible (import_title_);
    addAndMakeVisible (top_divider_);

    auto placeholder_colour = juce::Colours::grey.withAlpha (0.6f);
    name_input_.setTextToShowWhenEmpty ("Ir Name", placeholder_colour);
    name_input_.setMultiLine (false, false);
    addAndMakeVisible (name_input_);

    description_input_.setTextToShowWhenEmpty ("Ir Description", placeholder_colour);
    description_input_.setMultiLine (true, true);
    addAndMakeVisible (description_input_);

    Preferences preferences;
    preferences.Load ();

    auto user_paths = preferences.user_paths;
    for (auto i = 0; i < user_paths.size (); ++i)
        user_path_picker_.addItem (user_paths [i].string (), i + 1);
    user_path_picker_.setSelectedItemIndex (0);
    addAndMakeVisible (user_path_picker_);

    juce::StringArray microphone_picker_options;
    for (auto & kvp : kMicrophonePickerOptions)
        microphone_picker_options.add (kvp.first);
    microphone_array_picker_.addItemList (microphone_picker_options, 1);
    microphone_array_picker_.setSelectedItemIndex (0);

    addAndMakeVisible (microphone_array_picker_);

    speaker_positions_picker_.addItemList ({"C", "LR", "CLR"}, 1);
    speaker_positions_picker_.setSelectedItemIndex (0);
    auto update_speaker_positions = [&]
    {
        speaker_position_component_.UpdateVisiblePositions (
            static_cast<SpeakerPositionComponent::DisplayType> (
                speaker_positions_picker_.getSelectedItemIndex ()));
    };
    speaker_positions_picker_.onChange = update_speaker_positions;
    update_speaker_positions ();

    addAndMakeVisible (speaker_positions_picker_);

    addAndMakeVisible (speaker_positions_title_);
    addAndMakeVisible (middle_divider_);

    addAndMakeVisible (speaker_position_component_);

    import_ir_button_.onClick = [&]
    {
        if (OnSubmit)
            OnSubmit ();
    };
    addAndMakeVisible (import_ir_button_);
}

void ImportComponent::resized ()
{
    juce::FlexBox toolbar_layout;
    toolbar_layout.flexDirection = juce::FlexBox::Direction::row;
    toolbar_layout.items.add (juce::FlexItem (microphone_array_picker_).withFlex (1.0f));
    toolbar_layout.items.add (LookAndFeel::kFlexSpacer);
    toolbar_layout.items.add (juce::FlexItem (speaker_positions_picker_).withFlex (1.0f));

    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;

    layout.items.add (LookAndFeel::LabelFlexItem (import_title_));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (LookAndFeel::HorizontalDividerFlexItem (top_divider_));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (name_input_).withHeight (LookAndFeel::kButtonHeight));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (
        juce::FlexItem (description_input_).withHeight (LookAndFeel::kButtonHeight * 4));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (user_path_picker_).withHeight (LookAndFeel::kButtonHeight));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (toolbar_layout).withHeight (LookAndFeel::kButtonHeight));

    layout.items.add (LookAndFeel::LabelFlexItem (speaker_positions_title_));
    layout.items.add (LookAndFeel::HorizontalDividerFlexItem (middle_divider_));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (speaker_position_component_).withFlex (1.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (LookAndFeel::ButtonFlexItem (import_ir_button_));

    layout.performLayout (getLocalBounds ().toFloat ());
}

std::string ImportComponent::GetIrName () const
{
    return name_input_.getText ().toStdString ();
}

std::string ImportComponent::GetIrDescription () const
{
    return description_input_.getText ().toStdString ();
}

std::string ImportComponent::GetUserPath () const
{
    return user_path_picker_.getText ().toStdString ();
}
ChannelFormat ImportComponent::GetChannelFormat () const
{
    auto channel_format_string = microphone_array_picker_.getText ().toStdString ();
    if (kMicrophonePickerOptions.contains (channel_format_string))
        return kMicrophonePickerOptions.at (channel_format_string);

    jassertfalse;
    return ChannelFormat::kMono;
}
