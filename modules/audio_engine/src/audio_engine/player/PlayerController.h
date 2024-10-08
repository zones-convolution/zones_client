#pragma once

#include "../CommandQueue.h"
#include "PlayerState.h"

#include <rocket.hpp>

struct PlayerControllerNotificationDelegate
{
    virtual void ReceivedPlayerStateNotification (const Player::PlayerState & new_state) = 0;
};

class PlayerController : public PlayerControllerNotificationDelegate
{
public:
    PlayerController (CommandQueue::VisitorQueue & command_queue)
        : command_queue_ (command_queue)
    {
    }

    void SetPlayerState (Player::PlayerState state)
    {
        command_queue_.PushCommand (CommandQueue::SetPlayerStateCommand {state});
    }

    [[nodiscard]] Player::PlayerState GetPlayerState () const
    {
        return player_state_;
    }

    rocket::signal<void ()> OnPlayerStateUpdated;

    void ReceivedPlayerStateNotification (const Player::PlayerState & new_state) override
    {
        player_state_ = new_state;
        OnPlayerStateUpdated ();
    }

private:
    CommandQueue::VisitorQueue & command_queue_;
    Player::PlayerState player_state_;
};
