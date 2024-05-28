#pragma once
#include "audio_engine/player/PlayerController.h"
#include "juce_gui_extra/juce_gui_extra.h"
#include "look_and_feel/BoxIcons.h"
#include "look_and_feel/components/DividerComponent.h"
#include "look_and_feel/components/IconButton.h"

#include <juce_gui_basics/juce_gui_basics.h>

class PlayerComponent : public juce::Component
{
public:
    explicit PlayerComponent (PlayerController & player_controller);
    void resized () override;

private:
    void Update ();

    static const std::string kPlayerPanelKey;
    static const std::string kPlayerGainKey;

    PlayerController & player_controller_;

    juce::Label player_label_;
    DividerComponent top_divider_;
    juce::ComboBox file_chooser_;
    IconButton play_pause_button_ {"Play Pause", BoxIcons::kBxPlay};
    IconButton loop_button_ {"Loop", BoxIcons::kBxRecycle};

    juce::Label player_gain_label_;
    juce::Slider player_gain_slider_ {juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                                      juce::Slider::TextEntryBoxPosition::NoTextBox};
    // juce::ToggleButton loop_button_;
};
