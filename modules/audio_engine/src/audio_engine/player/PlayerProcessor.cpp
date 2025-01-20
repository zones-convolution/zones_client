#include "PlayerProcessor.h"

#include <juce_core/juce_core.h>

extern "C" const char resources_snare_mp3 [];
extern "C" const unsigned resources_snare_mp3_size;

extern "C" const char resources_voice_mp3 [];
extern "C" const unsigned resources_voice_mp3_size;

PlayerProcessor::PlayerProcessor (NotificationQueue::VisitorQueue & notification_queue)
    : notification_queue_ (notification_queue)
{
    reset ();
    audio_format_manager_.registerBasicFormats ();

    std::shared_ptr<juce::AudioFormatReader> snare_reader {
        audio_format_manager_.createReaderFor (std::make_unique<juce::MemoryInputStream> (
            &resources_snare_mp3, resources_snare_mp3_size, false))};

    readers_.push_back (snare_reader);

    std::shared_ptr<juce::AudioFormatReader> numbers_reader {
        audio_format_manager_.createReaderFor (std::make_unique<juce::MemoryInputStream> (
            &resources_voice_mp3, resources_voice_mp3_size, false))};

    readers_.push_back (numbers_reader);

    notification_queue_.PushCommand (player_state_);
}

void PlayerProcessor::prepare (const juce::dsp::ProcessSpec & spec)
{
    temp_buffer_.setSize (static_cast<int> (spec.numChannels),
                          static_cast<int> (spec.maximumBlockSize));
    smoothed_gain_.reset (spec.sampleRate, 0.05f);
    smoothed_gain_.setCurrentAndTargetValue (0.f);
}

void PlayerProcessor::process (const juce::dsp::ProcessContextReplacing<float> & replacing)
{
    if (! player_state_.playing)
        return;

    auto reader_index = static_cast<unsigned long> (player_state_.resource);
    auto & reader = readers_ [reader_index];
    auto output_block = replacing.getOutputBlock ();
    auto total_num_samples_to_collect = static_cast<int> (output_block.getNumSamples ());
    auto total_num_input_samples = static_cast<int> (reader->lengthInSamples);
    auto num_samples_collected = 0;

    while (num_samples_collected < total_num_samples_to_collect)
    {
        auto remaining_samples = total_num_input_samples - read_head_;
        auto num_samples_to_collect =
            std::min (remaining_samples, total_num_samples_to_collect - num_samples_collected);

        reader->read (
            &temp_buffer_, num_samples_collected, num_samples_to_collect, read_head_, true, true);

        num_samples_collected += num_samples_to_collect;
        read_head_ += num_samples_to_collect;

        if (read_head_ >= total_num_input_samples)
        {
            read_head_ = 0;

            if (! player_state_.looping)
            {
                player_state_.playing = false;
                smoothed_gain_.setCurrentAndTargetValue (0.f);
                notification_queue_.PushCommand (player_state_);
                break;
            }
        }
    }

    output_block.copyFrom (temp_buffer_).multiplyBy (smoothed_gain_);

    if (is_stopping_ && smoothed_gain_.getCurrentValue () == 0.f)
    {
        read_head_ = 0;
        player_state_.playing = false;
        player_state_.resource = new_resource_;
        is_stopping_ = false;

        notification_queue_.PushCommand (player_state_);
    }
}
void PlayerProcessor::reset ()
{
    player_state_.resource = Player::Resource::kSnare;
    player_state_.looping = false;
    player_state_.playing = false;
    player_state_.gain = 1.f;

    Clear ();
}

void PlayerProcessor::Clear ()
{
    smoothed_gain_.setTargetValue (0.f);
    is_stopping_ = false;
    read_head_ = 0;
}

void PlayerProcessor::SetPlayerState (const Player::PlayerState & new_player_state)
{
    player_state_.looping = new_player_state.looping;

    auto gain = std::clamp (new_player_state.gain, 0.f, 2.f);
    smoothed_gain_.setTargetValue (gain);
    player_state_.gain = gain;

    auto new_playing_state = new_player_state.playing;
    if (! new_playing_state)
    {
        is_stopping_ = true;
        smoothed_gain_.setTargetValue (0.f);
    }
    else
    {
        is_stopping_ = false;
        player_state_.playing = new_playing_state;
    }

    auto new_resource = new_player_state.resource;
    if (player_state_.resource != new_resource && player_state_.playing)
    {
        is_stopping_ = true;
        smoothed_gain_.setTargetValue (0.f);
        new_resource_ = new_resource;
    }
    else
    {
        player_state_.resource = new_resource;
    }

    notification_queue_.PushCommand (player_state_);
}
