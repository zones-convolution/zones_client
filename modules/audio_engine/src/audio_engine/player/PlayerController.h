#pragma once

#include "../CommandQueue.h"
#include "PlayerState.h"

class PlayerController
{
public:
    // using PlayerState = NotificationQueue::PlayerStateNotification;

    PlayerController (CommandQueue::VisitorQueue & command_queue)
        : command_queue_ (command_queue)
    {
    }

    void Play (Player::Resources file, bool looping)
    {
        command_queue_.PushCommand (CommandQueue::PlayCommand {.file = file, .looping = looping});
    }

    void Stop ()
    {
        command_queue_.PushCommand (CommandQueue::StopCommand {});
    }

    void SetLoop (bool looping)
    {
        command_queue_.PushCommand (CommandQueue::LoopCommand {.loop = looping});
    }

    void SetFile (Player::Resources file)
    {
        command_queue_.PushCommand (CommandQueue::FileCommand {.file = file});
    }

    [[nodiscard]] Player::PlayerState GetPlayerState () const
    {
        return player_state_;
    }

    std::function<void ()> OnPlayerStateUpdated;

    void ReceivedPlayerStateNotification (const Player::PlayerState & new_state)
    {
        player_state_ = new_state;
        OnPlayerStateUpdated ();
    }

private:
    CommandQueue::VisitorQueue & command_queue_;
    Player::PlayerState player_state_;
};
