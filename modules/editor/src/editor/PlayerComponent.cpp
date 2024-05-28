#include "PlayerComponent.h"

const std::string PlayerComponent::kPlayerPanelKey = "player_panel";
const std::string PlayerComponent::kPlayerGainKey = "player_gain";

PlayerComponent::PlayerComponent (PlayerController & player_controller)
    : player_controller_ (player_controller)
{
    auto state = player_controller.GetPlayerState ();

    player_label_.setText (juce::translate (kPlayerPanelKey), juce::dontSendNotification);
    addAndMakeVisible (player_label_);
    addAndMakeVisible (top_divider_);

    file_chooser_.addItemList ({"snare", "numbers"}, 1);
    file_chooser_.setSelectedItemIndex (state.file, juce::dontSendNotification);
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
                                     loop_button_.getToggleState (),
                                     player_gain_slider_.getValue ());
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

    player_gain_label_.setText (juce::translate (kPlayerGainKey), juce::dontSendNotification);
    player_gain_label_.setJustificationType (juce::Justification::centred);
    player_gain_slider_.setPopupDisplayEnabled (true, true, getTopLevelComponent ());
    player_gain_slider_.setRange (0.f, 2.f, 0.01f);
    auto gain = (! state.is_playing && state.gain == 0) ? 1.f : state.gain;
    player_gain_slider_.setValue (gain);
    player_gain_slider_.onValueChange = [this]
    { player_controller_.SetGain (player_gain_slider_.getValue ()); };
    addAndMakeVisible (player_gain_slider_);
    addAndMakeVisible (player_gain_label_);

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
    auto player_gain_layout =
        LookAndFeel::SliderLabelLayout (player_gain_slider_, player_gain_label_);

    juce::FlexBox button_layout;
    button_layout.flexDirection = juce::FlexBox::Direction::row;
    button_layout.items.add (LookAndFeel::ButtonFlexItem (play_pause_button_).withFlex (1.f));
    button_layout.items.add (LookAndFeel::kFlexSpacer);
    button_layout.items.add (LookAndFeel::ButtonFlexItem (loop_button_).withFlex (1.f));

    juce::FlexBox left_layout;
    left_layout.flexDirection = juce::FlexBox::Direction::column;
    left_layout.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    left_layout.items.add (juce::FlexItem (file_chooser_).withHeight (LookAndFeel::kButtonHeight));
    left_layout.items.add (juce::FlexItem (button_layout).withHeight (LookAndFeel::kButtonHeight));

    juce::FlexBox parameter_layout;
    parameter_layout.flexDirection = juce::FlexBox::Direction::row;
    parameter_layout.items.add (juce::FlexItem (left_layout).withFlex (1.f));
    parameter_layout.items.add (LookAndFeel::kFlexSpacer);
    parameter_layout.items.add (juce::FlexItem (player_gain_layout).withFlex (1.f));

    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;
    layout.items.add (LookAndFeel::LabelFlexItem (player_label_));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (LookAndFeel::HorizontalDividerFlexItem (top_divider_));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (parameter_layout).withFlex (1.f));

    layout.performLayout (getLocalBounds ().toFloat ());
}

void PlayerComponent::update ()
{
    auto new_state = player_controller_.GetPlayerState ();

    play_pause_button_.setToggleState (new_state.is_playing, juce::dontSendNotification);
    play_pause_button_.SetIcon (new_state.is_playing ? BoxIcons::kBxPause : BoxIcons::kBxPlay);

    loop_button_.setToggleState (new_state.is_looping, juce::dontSendNotification);

    player_gain_slider_.setValue (new_state.gain, juce::dontSendNotification);
}
