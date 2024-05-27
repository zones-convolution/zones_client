#include "PlayerComponent.h"

const std::string PlayerComponent::kPlayerPanelKey = "player_panel";

PlayerComponent::PlayerComponent (PlayerController & player_controller)
    : player_controller_ (player_controller)
{
    player_label_.setText (juce::translate (kPlayerPanelKey), juce::dontSendNotification);
    addAndMakeVisible (player_label_);
    addAndMakeVisible (top_divider_);

    file_chooser_.addItemList ({"snare", "numbers"}, 1);
    file_chooser_.setSelectedItemIndex (0, juce::dontSendNotification);
    file_chooser_.onChange = [this]
    {
        player_controller_.SetFile (
            static_cast<Player::Resources> (file_chooser_.getSelectedItemIndex ()));
    };
    addAndMakeVisible (file_chooser_);

    play_pause_button_.setToggleable (true);
    play_pause_button_.onClick = [this]
    {
        if (! play_pause_button_.getToggleState ())
        {
            auto file = file_chooser_.getSelectedItemIndex ();
            player_controller_.Play (static_cast<Player::Resources> (file),
                                     loop_button_.getToggleState ());
        }
        else
        {
            player_controller_.Stop ();
        }
    };
    addAndMakeVisible (play_pause_button_);

    loop_button_.setToggleable (true);
    loop_button_.setToggleState (false, juce::dontSendNotification);
    loop_button_.onClick = [this]
    {
        if (loop_button_.getToggleState ())
            player_controller_.SetLoop (true);
        else
        {
            player_controller_.SetLoop (false);
        }
    };
    addAndMakeVisible (loop_button_);

    player_controller_.OnPlayerStateUpdated = [this] { SetPlayerState (); };
    SetPlayerState ();
}

void PlayerComponent::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;
    //  layout.alignItems = juce::FlexBox::AlignItems::center;

    juce::FlexBox button_layout;
    button_layout.flexDirection = juce::FlexBox::Direction::row;
    button_layout.items.add (LookAndFeel::ButtonFlexItem (play_pause_button_).withFlex (1.f));
    button_layout.items.add (LookAndFeel::kFlexSpacer);
    button_layout.items.add (LookAndFeel::ButtonFlexItem (loop_button_).withFlex (1.f));

    layout.items.add (LookAndFeel::LabelFlexItem (player_label_));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (LookAndFeel::HorizontalDividerFlexItem (top_divider_));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (file_chooser_).withFlex (1.0f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (button_layout).withHeight (LookAndFeel::kButtonHeight));

    layout.performLayout (getLocalBounds ().toFloat ());
}

void PlayerComponent::SetPlayerState ()
{
    auto new_state = player_controller_.GetPlayerState ();
    //    text_ = {};
    //    text_ += new_state.is_playing.value () ? "playing" : "paused";
    //    text_ += " ";
    //    text_ += new_state.looping.value () ? "looping" : "one shot";
    //    text_ += " ";
    //    if (new_state.is_playing.has_value ())
    //        play_pause_button_.setToggleState (new_state.is_playing.value (),
    //                                           juce::dontSendNotification);
    // player_state_.setText (text_, juce::dontSendNotification);
    //  resized ();
}
