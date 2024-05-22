#include "PlayerProcessor.h"

#include <juce_core/juce_core.h>

extern "C" const char resources_snare_mp3 [];
extern "C" const unsigned resources_snare_mp3_size;

// static const juce::MemoryInputStream snare_stream {&resources_snare_wav,
//                                                    resources_snare_wav_size,
//                                                    false};

PlayerProcessor::PlayerProcessor (NotificationQueue::VisitorQueue & notification_queue)
    : notification_queue_ (notification_queue)
{
    reset ();
}

void PlayerProcessor::prepare (const juce::dsp::ProcessSpec & spec)
{
    reset ();
}
void PlayerProcessor::process (const juce::dsp::ProcessContextReplacing<float> & replacing)
{
}
void PlayerProcessor::reset ()
{
    player_state_.file = 0;
    player_state_.is_playing = false;
    player_state_.looping = false;
}

void PlayerProcessor::SetPlayerState (NotificationQueue::PlayerStateNotification new_player_state)
{
    player_state_ = new_player_state;
    notification_queue_.PushCommand (NotificationQueue::PlayerStateNotification {player_state_});
}
