#pragma once
#include "audio_engine/player/PlayerController.h"
#include "look_and_feel/BoxIcons.h"
#include "look_and_feel/components/DividerComponent.h"
#include "look_and_feel/components/IconButton.h"

#include <juce_gui_basics/juce_gui_basics.h>

class PlayerComponent : public juce::Component
{
public:
    PlayerComponent (PlayerController & player_controller);
    void resized () override;

private:
    static const std::string kPlayerPanelKey;
    void SetPlayerState ();

    PlayerController & player_controller_;

    juce::Label player_label_;
    DividerComponent top_divider_;

    juce::ComboBox file_chooser_;
    IconButton play_pause_button_ {"Play Pause", BoxIcons::kBxPlay};
    juce::ToggleButton loop_button_;
};
