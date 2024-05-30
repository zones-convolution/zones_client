#pragma once
#include "../NotificationQueue.h"
#include "PlayerState.h"

#include <juce_dsp/juce_dsp.h>

class PlayerProcessor : public juce::dsp::ProcessorBase
{
public:
    PlayerProcessor (NotificationQueue::VisitorQueue & notification_queue);
    void prepare (const juce::dsp::ProcessSpec & spec) override;
    void process (const juce::dsp::ProcessContextReplacing<float> & replacing) override;
    void reset () override;
    void Clear ();

    void SetPlayerState (Player::PlayerStateOptional new_player_state);

private:
    juce::AudioBuffer<float> temp_buffer_;
    juce::AudioFormatManager audio_format_manager_;
    std::vector<juce::AudioFormatReader *> readers_;

    int read_head_;
    NotificationQueue::VisitorQueue & notification_queue_;
    Player::PlayerState player_state_;
    bool is_stopping_;

    juce::LinearSmoothedValue<float> smoothed_gain_;
};
