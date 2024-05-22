#pragma once

#include "../CommandQueue.h"
#include "../NotificationQueue.h"

class PlayerController
{
public:
    using PlayerState = NotificationQueue::PlayerStateNotification;

    PlayerController (CommandQueue & command_queue);

    void Play (int file, int mode)
    {
        //        command_queue_.PushCommand (PlayCommand {});
    }

    void Stop ()
    {
        //        command_queue_.Queue ();
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
    CommandQueue & command_queue_;
    PlayerState player_state_;
};
