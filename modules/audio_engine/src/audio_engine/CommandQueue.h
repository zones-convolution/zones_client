#pragma once
#include "VisitorQueue.h"
#include "player/PlayerState.h"

#include <variant>

struct CommandQueue
{
    struct SetPlayerStateCommand
    {
        Player::PlayerState state;
    };

    using Commands = std::variant<SetPlayerStateCommand>;
    struct Visitor
    {
        virtual void operator() (const SetPlayerStateCommand & set_player_state_command) = 0;
    };

    using VisitorQueue = VisitorQueue<Commands, Visitor>;
};
