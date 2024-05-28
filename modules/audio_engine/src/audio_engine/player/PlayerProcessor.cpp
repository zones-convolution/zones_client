#include "PlayerProcessor.h"

#include <juce_core/juce_core.h>

extern "C" const char resources_snare_mp3 [];
extern "C" const unsigned resources_snare_mp3_size;

extern "C" const char resources_numbers_mp3 [];
extern "C" const unsigned resources_numbers_mp3_size;

// static const juce::MemoryInputStream snare_stream {&resources_snare_mp3,
// resources_snare_mp3_size, false
//}
//;

PlayerProcessor::PlayerProcessor (NotificationQueue::VisitorQueue & notification_queue)
    : notification_queue_ (notification_queue)
{
    reset ();
    audio_format_manager_.registerBasicFormats ();

    readers_.push_back (
        audio_format_manager_.createReaderFor (std::make_unique<juce::MemoryInputStream> (
            &resources_snare_mp3, resources_snare_mp3_size, false)));

    readers_.push_back (
        audio_format_manager_.createReaderFor (std::make_unique<juce::MemoryInputStream> (
            &resources_numbers_mp3, resources_numbers_mp3_size, false)));

    reader_sizes_.push_back (readers_ [0]->lengthInSamples);
    reader_sizes_.push_back (readers_ [1]->lengthInSamples);
    // problem with push command in constructor...
    // SetPlayerState ({});
}

void PlayerProcessor::prepare (const juce::dsp::ProcessSpec & spec)
{
    temp_buffer_.setSize (spec.numChannels, spec.maximumBlockSize);
    smoothed_gain_.reset (spec.sampleRate, 0.05f);
    smoothed_gain_.setCurrentAndTargetValue (0.f);
}
void PlayerProcessor::process (const juce::dsp::ProcessContextReplacing<float> & replacing)
{
    if (! player_state_.is_playing)
        return;

    int reader_index = player_state_.file;

    auto output_block = replacing.getOutputBlock ();
    auto total_num_samples_to_collect = static_cast<int> (output_block.getNumSamples ());
    auto total_num_input_samples = reader_sizes_ [reader_index];
    auto num_samples_collected = 0;

    while (num_samples_collected < total_num_samples_to_collect)
    {
        auto remaining_samples = total_num_input_samples - read_head_;
        auto num_samples_to_collect =
            std::min (remaining_samples, total_num_samples_to_collect - num_samples_collected);

        readers_ [reader_index]->read (
            &temp_buffer_, num_samples_collected, num_samples_to_collect, read_head_, true, true);

        num_samples_collected += num_samples_to_collect;
        read_head_ += num_samples_to_collect;

        if (read_head_ >= total_num_input_samples)
        {
            read_head_ = 0;

            if (! player_state_.is_looping)
            {
                player_state_.is_playing = false;
                smoothed_gain_.setCurrentAndTargetValue (0.f);
                notification_queue_.PushCommand (player_state_);
                break;
            }
        }
    }

    output_block.copyFrom (temp_buffer_).multiplyBy (smoothed_gain_);

    if (smoothed_gain_.getCurrentValue () == 0.f)
    {
        read_head_ = 0;
        player_state_.is_playing = false;
        notification_queue_.PushCommand (player_state_);
    }
}
void PlayerProcessor::reset ()
{
    player_state_.file = Player::Resources::kSnare;
    player_state_.is_looping = false;
    player_state_.is_playing = false;
    player_state_.gain = 1.f;

    Clear ();
}

void PlayerProcessor::Clear ()
{
    smoothed_gain_.setTargetValue (0.f);
    read_head_ = 0;
}

void PlayerProcessor::SetPlayerState (Player::PlayerStateOptional new_player_state)
{
    if (new_player_state.is_looping.has_value ())
        player_state_.is_looping = new_player_state.is_looping.value ();

    if (new_player_state.file.has_value ())
    {
        auto new_file = new_player_state.file.value ();
        if (player_state_.file != new_file && player_state_.is_playing)
        {
            smoothed_gain_.setTargetValue (0.f);
        }
        else
        {
            player_state_.file = new_file;
        }
    }

    if (new_player_state.gain.has_value ())
    {
        auto gain = std::clamp (new_player_state.gain.value (), 0.f, 2.f);
        smoothed_gain_.setTargetValue (gain);
        player_state_.gain = gain;
    }

    if (new_player_state.is_playing.has_value ())
    {
        auto new_playing_state = new_player_state.is_playing.value ();
        if (! new_playing_state)
        {
            smoothed_gain_.setTargetValue (0.f);
            notification_queue_.PushCommand (player_state_);
            return;
        }
        player_state_.is_playing = new_playing_state;
    }

    notification_queue_.PushCommand (player_state_);
}
