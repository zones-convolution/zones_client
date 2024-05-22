#pragma once

#include "../CommandQueue.h"
#include "../NotificationQueue.h"

class PlayerController
{
public:
    using PlayerState = NotificationQueue::PlayerStateNotification;

    PlayerController (CommandQueue::VisitorQueue & command_queue)
        : command_queue_ (command_queue)
    {
    }

    void Play (int file, bool looping)
    {
        command_queue_.PushCommand (CommandQueue::PlayCommand {.file = file, .looping = looping});
    }

    void Stop ()
    {
        command_queue_.PushCommand (CommandQueue::StopCommand {});
    }

    [[nodiscard]] PlayerState GetPlayerState () const
    {
        return player_state_;
    }

    std::function<void (const PlayerState & player_state)> OnPlayerStateUpdated;

    void
    ReceivedPlayerStateNotification (const NotificationQueue::PlayerStateNotification & new_state)
    {
        player_state_ = new_state;
        OnPlayerStateUpdated (player_state_);
    }

private:
    CommandQueue::VisitorQueue & command_queue_;
    PlayerState player_state_;
};
