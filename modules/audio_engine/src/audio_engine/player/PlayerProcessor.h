#pragma once
#include "../NotificationQueue.h"

#include <juce_dsp/juce_dsp.h>

class PlayerProcessor : public juce::dsp::ProcessorBase
{
public:
    using PlayerState = NotificationQueue::PlayerStateNotification;

    PlayerProcessor (NotificationQueue::VisitorQueue & notification_queue);
    void prepare (const juce::dsp::ProcessSpec & spec) override;
    void process (const juce::dsp::ProcessContextReplacing<float> & replacing) override;
    void reset () override;

    void SetPlayerState (NotificationQueue::PlayerStateNotification new_player_state);

private:
    NotificationQueue::VisitorQueue & notification_queue_;
    PlayerState player_state_;
};
