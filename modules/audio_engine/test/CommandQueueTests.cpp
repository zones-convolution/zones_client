#include "audio_engine/CommandQueue.h"

#include <catch2/catch_test_macros.hpp>

class MockCommandQueueDelegate : public CommandQueue::Delegate
{
public:
    void RTLoadIr (IrData && ir_data) override
    {
        rt_load_ir_call_count_ += 1;
        rt_load_irs_.push_back (ir_data);
    }

    void RTUpdateParameters () override
    {
        rt_update_parameters_call_count_ += 1;
    }

    int rt_load_ir_call_count_ = 0;
    std::vector<IrData> rt_load_irs_;

    int rt_update_parameters_call_count_ = 0;
};

SCENARIO ("queues commands", "[CommandQueue]")
{
    static constexpr double kSampleRate = 48000.0;
    static constexpr double kBitDepth = 16.0;
    static constexpr int kNumChannels = 1;
    static constexpr int kNumSamples = 1;
    GIVEN ("a queued load ir command on a command queue")
    {
        MockCommandQueueDelegate delegate;
        CommandQueue command_queue {delegate};

        auto ir_data = IrData {
            .buffer = juce::AudioBuffer<float> (kNumChannels, kNumSamples),
            .sample_rate = kSampleRate,
            .bit_depth = kBitDepth,
        };

        command_queue.LoadIr (std::move (ir_data));

        WHEN ("servicing the queue")
        {
            command_queue.RTService ();

            THEN ("then the corresponding callback is invoked")
            {
                REQUIRE (delegate.rt_load_ir_call_count_ == 1);
                auto & last_loaded_ir_data = delegate.rt_load_irs_ [0];
                REQUIRE (juce::approximatelyEqual (last_loaded_ir_data.sample_rate, kSampleRate));
                REQUIRE (juce::approximatelyEqual (last_loaded_ir_data.bit_depth, kBitDepth));
                REQUIRE (last_loaded_ir_data.buffer.getNumChannels () == kNumChannels);
                REQUIRE (last_loaded_ir_data.buffer.getNumSamples () == kNumSamples);
            }
        }
    }

    GIVEN ("multiple queued ir commands on a command queue")
    {
        static constexpr int kNumCommandsToQueue = 20;
        MockCommandQueueDelegate delegate;
        CommandQueue command_queue {delegate};

        for (auto command_index = 0; command_index < kNumCommandsToQueue; ++command_index)
        {
            command_queue.LoadIr (IrData {.buffer = {kNumSamples, kNumChannels},
                                          .sample_rate = (double) command_index,
                                          .bit_depth = kBitDepth});
        }

        WHEN ("servicing the queue")
        {
            command_queue.RTService ();
            THEN ("commands are in the correct order")
            {
                for (auto command_index = 0; command_index < kNumCommandsToQueue; ++command_index)
                {
                    auto & ir_data = delegate.rt_load_irs_ [command_index];
                    REQUIRE (
                        juce::approximatelyEqual (ir_data.sample_rate, (double) command_index));
                }
            }
        }
    }
}
