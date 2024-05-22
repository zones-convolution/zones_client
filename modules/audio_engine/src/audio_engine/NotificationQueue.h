#pragma once

#pragma once
#include "VisitorQueue.h"

#include <variant>

struct NotificationQueue
{
    struct PlayerStateNotification
    {
        int file;
        bool looping;
        bool is_playing;
    };

    using Notifications = std::variant<PlayerStateNotification>;
    struct Visitor
    {
        virtual void operator() (const PlayerStateNotification & player_state_notification) = 0;
    };

    using VisitorQueue = VisitorQueue<Notifications, Visitor>;
};
