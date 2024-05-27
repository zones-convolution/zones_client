#pragma once

#pragma once
#include "VisitorQueue.h"
#include "player/PlayerController.h"
#include "player/PlayerState.h"

#include <variant>

struct NotificationQueue
{
    using Notifications = std::variant<Player::PlayerState>;
    struct Visitor
    {
        virtual void operator() (const Player::PlayerState & player_state_notification) = 0;
    };

    using VisitorQueue = VisitorQueue<Notifications, Visitor>;
};
