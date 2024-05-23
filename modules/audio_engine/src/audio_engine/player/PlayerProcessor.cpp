#include "PlayerProcessor.h"

#include <juce_core/juce_core.h>

extern "C" const char resources_snare_mp3 [];
extern "C" const unsigned resources_snare_mp3_size;

// static const juce::MemoryInputStream snare_stream {&resources_snare_mp3,
// resources_snare_mp3_size, false
//}
//;

PlayerProcessor::PlayerProcessor (NotificationQueue::VisitorQueue & notification_queue)
    : notification_queue_ (notification_queue)
{
    reset ();
    audio_format_manager_.registerBasicFormats ();

    reader_ = audio_format_manager_.createReaderFor (std::make_unique<juce::MemoryInputStream> (
        &resources_snare_mp3, resources_snare_mp3_size, false));

    SetPlayerState ({.is_playing = false, .looping = false});
}

void PlayerProcessor::prepare (const juce::dsp::ProcessSpec & spec)
{
    temp_buffer_.setSize (spec.numChannels, spec.maximumBlockSize);
    reset ();
}
void PlayerProcessor::process (const juce::dsp::ProcessContextReplacing<float> & replacing)
{
    if (! player_state_.is_playing.value ())
        return;

    auto output_block = replacing.getOutputBlock ();
    auto total_num_samples_to_collect = static_cast<int> (output_block.getNumSamples ());
    auto total_num_input_samples = static_cast<int> (resources_snare_mp3_size);
    auto num_samples_collected = 0;

    while (num_samples_collected < total_num_samples_to_collect)
    {
        auto remaining_samples = total_num_input_samples - read_head_;
        auto num_samples_to_collect =
            std::min (remaining_samples, total_num_samples_to_collect - num_samples_collected);

        reader_->read (
            &temp_buffer_, num_samples_collected, num_samples_to_collect, read_head_, true, true);

        num_samples_collected += num_samples_to_collect;
        read_head_ += num_samples_to_collect;

        if (read_head_ >= total_num_input_samples)
        {
            read_head_ = 0;

            if (! player_state_.looping.value ())
            {
                SetPlayerState ({.is_playing = false});
                output_block.copyFrom (temp_buffer_);
                return;
            }
        }
    }

    output_block.copyFrom (temp_buffer_);
}
void PlayerProcessor::reset ()
{
    player_state_.file = 0;
    player_state_.is_playing = false;
    player_state_.looping = false;

    read_head_ = 0;
}

void PlayerProcessor::SetPlayerState (NotificationQueue::PlayerStateNotification new_player_state)
{
    if (new_player_state.is_playing.has_value ())
    {
        player_state_.is_playing = new_player_state.is_playing.value ();
        if (! new_player_state.is_playing.value ())
            reset ();
    }

    if (new_player_state.looping.has_value ())
        player_state_.looping = new_player_state.looping.value ();

    if (new_player_state.file.has_value ())
        player_state_.file = new_player_state.file.value ();

    notification_queue_.PushCommand (NotificationQueue::PlayerStateNotification {player_state_});
}
