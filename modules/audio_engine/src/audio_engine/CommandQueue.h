#pragma once
#include "VisitorQueue.h"
#include "player/PlayerState.h"

#include <variant>

struct CommandQueue
{
    struct UpdateParameters
    {
        float dry_wet_mix;
        float input_gain;
        float output_gain;
        float bass;
        float treble;
    };

    struct SetPlayerStateCommand
    {
        Player::PlayerState state;
    };

    using Commands = std::variant<UpdateParameters, SetPlayerStateCommand>;
    struct Visitor
    {
        virtual void operator() (const UpdateParameters & update_parameters) = 0;
        virtual void operator() (const SetPlayerStateCommand & set_player_state_command) = 0;
    };

    using VisitorQueue = VisitorQueue<Commands, Visitor>;
};
