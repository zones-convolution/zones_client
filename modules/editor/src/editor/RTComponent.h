#pragma once
#include "EQComponent.h"
#include "IOComponent.h"
#include "PlayerComponent.h"
#include "audio_engine/player/PlayerController.h"
#include "look_and_feel/BoxIcons.h"
#include "look_and_feel/components/DividerComponent.h"
#include "look_and_feel/components/IconButton.h"
#include "look_and_feel/components/PanelComponent.h"
#include "model/ParameterTree.h"

#include <juce_gui_basics/juce_gui_basics.h>

class RTComponent : public juce::Component
{
public:
    explicit RTComponent (juce::AudioProcessorValueTreeState & parameter_tree,
                          PlayerController & player_controller);

    void resized () override;

private:
    PlayerComponent player_component_;
    PanelComponent player_panel_ {player_component_};

    EQComponent eq_component_;
    PanelComponent eq_panel_ {eq_component_};

    IOComponent io_component_;
    PanelComponent io_panel_ {io_component_};
};
