#include "ir_engine/IrGraph.h"
#include "ir_engine/processors/TestProcessor.h"

#include <catch2/catch_test_macros.hpp>

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

SCENARIO ("graphs can be processed")
{
    GIVEN ("an empty graph")
    {
        auto empty_graph = IrGraph ();
        WHEN ("process is called")
        {
            ProcessResultPool process_result_pool;
            auto process_result = empty_graph.Process ({}, process_result_pool);
            THEN ("it yields no result")
            {
                REQUIRE (process_result == std::nullopt);
            }
        }
    }

    GIVEN ("a graph with a single processor")
    {
        auto test_processor = std::make_shared<TestProcessor> ();
        auto test_processor_policy =
            IrGraph::CachePolicy ().WithPolicyIdentifier ("test_processor");
        auto ir_graph = IrGraph ().WithProcessor ({test_processor_policy, test_processor});

        WHEN ("process is called")
        {
            ProcessResultPool process_result_pool;

            auto process_result = ir_graph.Process ({}, process_result_pool);
            auto graph_keys = ir_graph.GetKeysForState ({});

            THEN ("a valid shared buffer is yielded")
            {
                REQUIRE (process_result.has_value ());
            }

            THEN ("a result is cached in the pool")
            {
                REQUIRE (process_result_pool.GetPoolSize () == 1);
                REQUIRE (process_result_pool.GetResult (graph_keys.back ()) == process_result);
            }
        }
    }
}