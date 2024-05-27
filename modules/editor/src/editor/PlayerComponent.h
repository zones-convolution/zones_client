#pragma once
#include "audio_engine/player/PlayerController.h"
#include "juce_gui_extra/juce_gui_extra.h"
#include "look_and_feel/BoxIcons.h"
#include "look_and_feel/components/DividerComponent.h"
#include "look_and_feel/components/IconButton.h"

#include <juce_gui_basics/juce_gui_basics.h>

class PlayerComponent : public juce::AnimatedAppComponent
{
public:
    PlayerComponent (PlayerController & player_controller);
    void resized () override;
    void update () override;
    void paint (juce::Graphics & g) override;

private:
    static const std::string kPlayerPanelKey;

    PlayerController & player_controller_;

    juce::Label player_label_;
    DividerComponent top_divider_;
    juce::ComboBox file_chooser_;
    IconButton play_pause_button_ {"Play Pause", BoxIcons::kBxPlay};
    IconButton loop_button_ {"Loop", BoxIcons::kBxRecycle};
    // juce::ToggleButton loop_button_;
};
