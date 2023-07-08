#include "MockProcessor.h"
#include "ir_engine/IrEngine.h"

#include <catch2/catch_test_macros.hpp>

SCENARIO ("ir engine jobs execute correctly")
{
    GIVEN ("an ir engine job with an empty graph")
    {
        auto ir_graph = IrGraph ();
        auto result_pool = ProcessResultPool ();
        bool did_execute_callback = false;
        auto job = IrEngine::Job (
            ir_graph, {}, result_pool, [&] (auto &&) { did_execute_callback = true; });
        WHEN ("when the job is run")
        {
            job.runJob ();
            THEN ("then the callback is not executed")
            {
                REQUIRE_FALSE (did_execute_callback);
            }
        }
    }

    GIVEN ("an ir engine job with a single processor")
    {
        auto mock_processor = std::make_shared<MockProcessor> ();
        auto copy_buffer = juce::AudioBuffer<float> {1, 1};
        copy_buffer.getWritePointer (0) [0] = 1.f;
        mock_processor->buffer_to_copy_from = copy_buffer;

        auto ir_graph = IrGraph ().WithProcessor (
            {IrGraph::CachePolicy ().WithPolicyIdentifier ("mock_policy"), mock_processor});

        auto result_pool = ProcessResultPool ();
        IrGraphProcessor::BoxedBuffer callback_result;

        auto job = IrEngine::Job (
            ir_graph, {}, result_pool, [&] (auto result) { callback_result = result; });

        WHEN ("when the job is run")
        {
            job.runJob ();
            THEN ("then the callback hands over the last processor result")
            {
                REQUIRE (callback_result.get ().getNumChannels () == copy_buffer.getNumChannels ());
                REQUIRE (callback_result.get ().getNumSamples () == copy_buffer.getNumSamples ());
                REQUIRE (juce::approximatelyEqual (callback_result.get ().getSample (0, 0),
                                                   copy_buffer.getSample (0, 0)));
            }
        }
    }
}