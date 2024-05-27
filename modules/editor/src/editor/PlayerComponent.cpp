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
    loop_button_.setClickingTogglesState (false);
    // loop_button_.onStateChange = [this] { loop_button_.set };
    loop_button_.onClick = [this]
    { player_controller_.SetLoop (! loop_button_.getToggleState ()); };
    addAndMakeVisible (loop_button_);

    setSynchroniseToVBlank (true);
}

void PlayerComponent::paint (juce::Graphics & g)
{
    g.fillAll (getLookAndFeel ().findColour (LookAndFeel::ColourIds::kPanel));

    //    auto channels_bounds = GetChannelBounds ().expanded (kMargin);
    //    g.setColour (getLookAndFeel ().findColour (juce::ResizableWindow::backgroundColourId));
    //    g.fillRect (channels_bounds);
}

void PlayerComponent::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;
    //  layout.alignItems = juce::FlexBox::AlignItems::center;

    juce::FlexBox button_layout;
    button_layout.flexDirection = juce::FlexBox::Direction::row;
    button_layout.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    button_layout.items.add (
        LookAndFeel::ButtonFlexItem (play_pause_button_).withWidth (LookAndFeel::kButtonHeight));
    button_layout.items.add (
        LookAndFeel::ButtonFlexItem (loop_button_).withWidth (LookAndFeel::kButtonHeight));

    layout.items.add (LookAndFeel::LabelFlexItem (player_label_));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (LookAndFeel::HorizontalDividerFlexItem (top_divider_));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (file_chooser_).withFlex (1.0f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (button_layout).withHeight (LookAndFeel::kButtonHeight));

    layout.performLayout (getLocalBounds ().toFloat ());
}

void PlayerComponent::update ()
{
    auto new_state = player_controller_.GetPlayerState ();

    play_pause_button_.setToggleState (new_state.is_playing, juce::dontSendNotification);
    play_pause_button_.SetIcon (new_state.is_playing ? BoxIcons::kBxPause : BoxIcons::kBxPlay);

    loop_button_.setToggleState (new_state.is_looping, juce::dontSendNotification);
}
