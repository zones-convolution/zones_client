#include "PreferencesComponent.h"

const juce::String PreferencesComponent::kPathPickerDialogTitle = "Pick Project Directory";

UserPathComponent::UserPathComponent (const std::filesystem::path & user_path)
    : user_path_ (user_path)
{
    path_label_.setText (user_path_.string (), juce::dontSendNotification);
    addAndMakeVisible (path_label_);

    remove_path_button_.onClick = [&] { OnPathRemoved (); };
    addAndMakeVisible (remove_path_button_);

    reveal_path_.onClick = [&] { juce::File (user_path_.string ()).revealToUser (); };
    addAndMakeVisible (reveal_path_);
}

void UserPathComponent::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::row;

    layout.items.add (LookAndFeel::LabelFlexItem (path_label_));
    layout.items.add (juce::FlexItem ().withFlex (1.0f));
    layout.items.add (LookAndFeel::MediumIconFlexItem (remove_path_button_));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (LookAndFeel::MediumIconFlexItem (reveal_path_));

    layout.performLayout (getLocalBounds ().toFloat ().reduced (LookAndFeel::kPadding));
}

PreferencesComponent::PreferencesComponent (lager::context<Action> & context)
    : context_ (context)
{
    addAndMakeVisible (preferences_label_);
    addAndMakeVisible (top_divider_);

    addAndMakeVisible (refresh_button_);
    addAndMakeVisible (add_path_button_);

    add_path_button_.onClick = [&] { AddPath (); };
    refresh_button_.onClick = [&] { UpdatePreferences (); };

    UpdatePreferences ();
}

void PreferencesComponent::AddPath ()
{
    directory_picker_ = std::make_unique<juce::FileChooser> (kPathPickerDialogTitle);
    auto directory_flags =
        juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectDirectories;

    directory_picker_->launchAsync (
        directory_flags,
        [&] (const juce::FileChooser & chooser)
        {
            auto path = chooser.getResult ();
            if (path.exists ())
            {
                auto & user_paths = preferences_.user_paths;
                user_paths.emplace_back (path.getFullPathName ().toStdString ());

                std::sort (user_paths.begin (), user_paths.end ());
                user_paths.erase (std::unique (user_paths.begin (), user_paths.end ()),
                                  user_paths.end ());

                preferences_.Save ();
                UpdatePreferences ();
            }
        });
}

void PreferencesComponent::UpdatePreferences ()
{
    preferences_.Load ();

    for (auto & user_path_label : user_path_components_)
        removeChildComponent (user_path_label.get ());

    user_path_components_.clear ();

    for (auto path_index = 0; path_index < preferences_.user_paths.size (); ++path_index)
    {
        auto & user_path = preferences_.user_paths [path_index];

        auto user_path_component = std::make_unique<UserPathComponent> (user_path);
        user_path_component->OnPathRemoved = [&, path_index]
        {
            preferences_.user_paths.erase (preferences_.user_paths.begin () + path_index);
            preferences_.Save ();
            UpdatePreferences ();
        };

        addAndMakeVisible (*user_path_component);
        user_path_components_.push_back (std::move (user_path_component));
    }

    context_.dispatch (RefreshUserZonesAction {});
    resized ();
}

void PreferencesComponent::resized ()
{
    juce::FlexBox actions_layout;
    actions_layout.flexDirection = juce::FlexBox::Direction::row;
    actions_layout.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;

    actions_layout.items.add (juce::FlexItem (add_path_button_).withWidth (100.0f));
    actions_layout.items.add (juce::FlexItem (refresh_button_).withWidth (120.0f));

    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;
    layout.items.add (LookAndFeel::LabelFlexItem (preferences_label_));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (LookAndFeel::HorizontalDividerFlexItem (top_divider_));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (actions_layout).withHeight (LookAndFeel::kButtonHeight));

    for (auto & user_path_component : user_path_components_)
    {
        layout.items.add (LookAndFeel::kFlexSpacer);
        layout.items.add (
            juce::FlexItem (*user_path_component).withHeight (LookAndFeel::kButtonHeight));
    }

    layout.performLayout (getLocalBounds ().toFloat ());
}
