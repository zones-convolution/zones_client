#include "MockProcessor.h"
#include "ir_engine/IrGraph.h"
#include "ir_engine/processors/TestProcessor.h"

#include <catch2/catch_test_macros.hpp>

auto never_abort_callback = [] () { return false; };

TEST_CASE ("getting keys for different state", "[IrGraph]")
{
    SECTION ("when graph has no processors there are no keys")
    {
        auto ir_graph = IrGraph ();
        IrGraphState state;
        REQUIRE (ir_graph.GetKeysForState (state).empty ());
    }

    SECTION ("processors have correct indexes and keys accumulate policies from past processors")
    {
        std::shared_ptr<IrGraphProcessor> processor_1 = std::make_shared<TestProcessor> ();
        auto policy_1 = IrGraph::CachePolicy ().WithPolicyIdentifier ("processor_1");

        std::shared_ptr<IrGraphProcessor> processor_2 = std::make_shared<TestProcessor> ();
        auto policy_2 = IrGraph::CachePolicy ().WithPolicyIdentifier ("processor_2");

        auto ir_graph = IrGraph ()
                            .WithProcessor ({policy_1, processor_1})
                            .WithProcessor ({policy_2, processor_2});

        auto state = IrGraphState {.param_1 = 9083.f};
        auto keys = ir_graph.GetKeysForState (state);

        REQUIRE (keys.size () == 2);

        REQUIRE (keys [0].graph_state->param_1 == state.param_1);
        REQUIRE (keys [1].graph_state->param_1 == state.param_1);

        REQUIRE (keys [0].policies.size () == 1);
        auto indexed_policy_1 = keys [0].policies.back ();
        REQUIRE (indexed_policy_1.cache_policy == policy_1);
        REQUIRE (indexed_policy_1.processor_index == 0);

        REQUIRE (keys [1].policies.size () == 2);
        auto indexed_policy_2 = keys [1].policies.back ();
        REQUIRE (indexed_policy_2.cache_policy == policy_2);
        REQUIRE (indexed_policy_2.processor_index == 1);

        auto indexed_policy_2_previous = keys [1].policies [0];
        REQUIRE (indexed_policy_2_previous.cache_policy == policy_1);
        REQUIRE (indexed_policy_2_previous.processor_index == 0);
    }
}

SCENARIO ("graphs can be processed", "[IrGraph]")
{
    GIVEN ("an empty graph")
    {
        auto empty_graph = IrGraph ();
        WHEN ("process is called")
        {
            ProcessResultPool process_result_pool;
            auto process_result =
                empty_graph.Process ({}, process_result_pool, never_abort_callback);
            THEN ("it yields no result")
            {
                REQUIRE (process_result == std::nullopt);
            }
        }
    }

    GIVEN ("a graph with a single processor")
    {
        auto mock_processor = std::make_shared<MockProcessor> ();
        auto mock_processor_policy =
            IrGraph::CachePolicy ().WithPolicyIdentifier ("mock_processor");
        auto ir_graph = IrGraph ().WithProcessor ({mock_processor_policy, mock_processor});

        mock_processor->buffer_to_copy_from.setSize (1, 1);
        juce::dsp::AudioBlock<float> block_to_copy {mock_processor->buffer_to_copy_from};
        static const float kSampleToCopy = 10.f;
        block_to_copy.setSample (0, 0, kSampleToCopy);

        WHEN ("process is called")
        {
            ProcessResultPool process_result_pool;

            auto process_result = ir_graph.Process ({}, process_result_pool, never_abort_callback);
            auto graph_keys = ir_graph.GetKeysForState ({});

            THEN ("an empty buffer is given as input to the first processor")
            {
                REQUIRE (mock_processor->last_input_buffer->getNumChannels () == 0);
            }

            THEN ("the mock processors result is yielded")
            {
                REQUIRE (mock_processor->process_call_count == 1);
                REQUIRE (process_result.has_value ());
                auto result_sample = process_result->get ().getSample (0, 0);
                REQUIRE (juce::approximatelyEqual (result_sample, kSampleToCopy));
            }

            THEN ("a result is cached in the pool")
            {
                REQUIRE (process_result_pool.GetPoolSize () == 1);
                REQUIRE (process_result_pool.GetResult (graph_keys.back ())->impl () ==
                         process_result->impl ());
            }
        }
    }

    GIVEN ("a graph with multiple processors")
    {
        auto processor_1 = std::make_shared<MockProcessor> ();
        auto policy_1 = IrGraph::CachePolicy ().WithPolicyIdentifier ("processor_1");

        static const float kSampleToCopyP1 = 1.f;
        processor_1->buffer_to_copy_from.setSize (1, 1);
        processor_1->buffer_to_copy_from.setSample (0, 0, kSampleToCopyP1);

        auto processor_2 = std::make_shared<MockProcessor> ();
        auto policy_2 = IrGraph::CachePolicy ().WithPolicyIdentifier ("processor_2");

        static const float kSampleToCopyP2 = 2.f;
        processor_2->buffer_to_copy_from.setSize (1, 1);
        processor_2->buffer_to_copy_from.setSample (0, 0, kSampleToCopyP2);

        auto ir_graph = IrGraph ()
                            .WithProcessor ({policy_1, processor_1})
                            .WithProcessor ({policy_2, processor_2});

        WHEN ("process is called")
        {
            ProcessResultPool process_result_pool;
            auto process_result = ir_graph.Process ({}, process_result_pool, never_abort_callback);
            auto graph_keys = ir_graph.GetKeysForState ({});

            THEN ("the pool is populated with two results")
            {
                REQUIRE (process_result_pool.GetPoolSize () == 2);

                REQUIRE (juce::approximatelyEqual (
                    process_result_pool.GetResult (graph_keys [0])->get ().getSample (0, 0),
                    kSampleToCopyP1));
                REQUIRE (juce::approximatelyEqual (
                    process_result_pool.GetResult (graph_keys [1])->get ().getSample (0, 0),
                    kSampleToCopyP2));
            }

            THEN ("both processors are processed once")
            {
                REQUIRE (processor_1->process_call_count == 1);
                REQUIRE (processor_2->process_call_count == 1);
            }

            THEN ("the correct inputs are given to the processors")
            {
                REQUIRE (processor_1->last_input_buffer->getNumChannels () == 0);

                REQUIRE (processor_2->last_input_buffer.impl () ==
                         process_result_pool.GetResult (graph_keys [0])->impl ());
            }

            THEN ("the result of the last processor is yileded")
            {
                REQUIRE (process_result->impl () ==
                         process_result_pool.GetResult (graph_keys.back ()).value ().impl ());
            }

            AND_WHEN ("process is called again with the same state")
            {
                process_result = ir_graph.Process ({}, process_result_pool, never_abort_callback);

                THEN ("processors are not re-processed")
                {
                    REQUIRE (processor_1->process_call_count == 1);
                    REQUIRE (processor_2->process_call_count == 1);
                }

                THEN ("the correct process result is yielded")
                {
                    REQUIRE (process_result->impl () ==
                             process_result_pool.GetResult (graph_keys.back ()).value ().impl ());
                    REQUIRE (juce::approximatelyEqual (process_result->get ().getSample (0, 0),
                                                       kSampleToCopyP2));
                }
            }
        }
    }
}

SCENARIO ("graph uses cached results in process call", "[IrGraph]")
{
    GIVEN ("a graph with multiple processors")
    {
        auto processor_1 = std::make_shared<MockProcessor> ();
        auto policy_1 = IrGraph::CachePolicy ().WithPolicyIdentifier ("processor_1");

        static const float kSampleToCopyP1 = 1.f;
        processor_1->buffer_to_copy_from.setSize (1, 1);
        processor_1->buffer_to_copy_from.setSample (0, 0, kSampleToCopyP1);

        auto processor_2 = std::make_shared<MockProcessor> ();
        auto policy_2 = IrGraph::CachePolicy ()
                            .WithPolicyIdentifier ("processor_2")
                            .WithCachedHandle (&IrGraphState::CacheParam1);

        static const float kSampleToCopyP2 = 2.f;
        processor_2->buffer_to_copy_from.setSize (1, 1);
        processor_2->buffer_to_copy_from.setSample (0, 0, kSampleToCopyP2);

        auto processor_3 = std::make_shared<MockProcessor> ();
        auto policy_3 = IrGraph::CachePolicy ().WithPolicyIdentifier ("processor_3");

        static const float kSampleToCopyP3 = 2.f;
        processor_3->buffer_to_copy_from.setSize (1, 1);
        processor_3->buffer_to_copy_from.setSample (0, 0, kSampleToCopyP3);

        auto ir_graph = IrGraph ()
                            .WithProcessor ({policy_1, processor_1})
                            .WithProcessor ({policy_2, processor_2})
                            .WithProcessor ({policy_3, processor_3});

        WHEN ("process is called with an initial state")
        {
            ProcessResultPool process_result_pool;
            auto process_result = ir_graph.Process ({}, process_result_pool, never_abort_callback);
            auto last_graph_keys = ir_graph.GetKeysForState ({});

            THEN ("the result pool is populated with 3 results")
            {
                REQUIRE (process_result_pool.GetPoolSize () == 3);
            }

            AND_WHEN ("the state is updated with respect to policy_2")
            {
                auto new_state = IrGraphState {.param_1 = 10.f};
                auto graph_keys = ir_graph.GetKeysForState (new_state);
                process_result =
                    ir_graph.Process (new_state, process_result_pool, never_abort_callback);

                THEN ("the correct processors are re-processed")
                {
                    REQUIRE (processor_1->process_call_count == 1);
                    REQUIRE (processor_2->process_call_count == 2);
                    REQUIRE (processor_3->process_call_count == 2);
                }

                THEN ("the correct keys have updated")
                {
                    REQUIRE (graph_keys [0] == last_graph_keys [0]);
                    REQUIRE (graph_keys [1] != last_graph_keys [1]);
                    REQUIRE (graph_keys [2] != last_graph_keys [2]);
                }

                THEN ("the process result pool contains 5 results")
                {
                    REQUIRE (process_result_pool.GetPoolSize () == 5);
                }

                THEN ("processors are given the correct input")
                {
                    REQUIRE (processor_2->last_input_buffer.impl () ==
                             process_result_pool.GetResult (graph_keys [0])->impl ());

                    REQUIRE (processor_3->last_input_buffer.impl () ==
                             process_result_pool.GetResult (graph_keys [1])->impl ());
                }

                THEN ("the correct process result is yielded")
                {
                    REQUIRE (process_result->impl () ==
                             process_result_pool.GetResult (graph_keys.back ()).value ().impl ());
                    REQUIRE (juce::approximatelyEqual (process_result->get ().getSample (0, 0),
                                                       kSampleToCopyP2));
                }
            }
        }
    }
}
//
// SCENARIO ("graph obeys abort callback")
//{
//    GIVEN ("a graph")
//    {
//        ProcessResultPool process_result_pool;
//
//        std::shared_ptr<IrGraphProcessor> processor_1 = std::make_shared<TestProcessor> ();
//        auto policy_1 = IrGraph::CachePolicy ().WithPolicyIdentifier ("processor_1");
//
//        std::shared_ptr<IrGraphProcessor> processor_2 = std::make_shared<TestProcessor> ();
//        auto policy_2 = IrGraph::CachePolicy ().WithPolicyIdentifier ("processor_2");
//
//        auto ir_graph = IrGraph ()
//                            .WithProcessor ({policy_1, processor_1})
//                            .WithProcessor ({policy_2, processor_2});
//
//        int abort_callback_call_count = 0;
//
//        WHEN ("process is called with an aborting callback")
//        {
//            auto result = ir_graph.Process ({},
//                                            process_result_pool,
//                                            [&] ()
//                                            {
//                                                abort_callback_call_count += 1;
//                                                return true;
//                                            });
//            THEN ("the result is empty")
//            {
//                REQUIRE (result == std::nullopt);
//            }
//
//            THEN ("the callback is only called once")
//            {
//                REQUIRE (abort_callback_call_count == 1);
//            }
//        }
//
//        WHEN ("process is called")
//        {
//            auto result = ir_graph.Process ({},
//                                            process_result_pool,
//                                            [&] ()
//                                            {
//                                                abort_callback_call_count += 1;
//                                                return false;
//                                            });
//
//            THEN ("the callback is called twice")
//            {
//                REQUIRE (abort_callback_call_count == 2);
//            }
//        }
//    }
//}