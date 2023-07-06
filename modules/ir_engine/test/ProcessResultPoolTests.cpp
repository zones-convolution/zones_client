#include "ir_engine/IrGraph.h"
#include "ir_engine/IrGraphStateKey.h"
#include "ir_engine/ProcessResultPool.h"

#include <catch2/catch_test_macros.hpp>

SCENARIO ("pool can cache results", "[ProcessResultPool]")
{
    GIVEN ("an empty pool")
    {
        ProcessResultPool process_result_pool;

        THEN ("the size of the pool is 0")
        {
            REQUIRE (process_result_pool.GetPoolSize () == 0);
        }

        THEN ("retrieving a key yields a null result")
        {
            REQUIRE (process_result_pool.GetResult ({}) == std::nullopt);
        }

        WHEN ("a result is cached")
        {
            auto key = GraphStateKey ().WithGraphState ({}).WithIndexedPolicy (
                {.cache_policy = {IrGraph::CachePolicy ().WithPolicyIdentifier ("test_policy")},
                 .processor_index = 0});
            IrGraphProcessor::BoxedBuffer result {};
            process_result_pool.CacheResult (key, result);

            THEN ("its value can be retrieved using the same key")
            {
                REQUIRE (process_result_pool.GetResult (key)->impl () == result.impl ());
            }

            AND_WHEN ("a different result is cached")
            {
                auto key_different = GraphStateKey ().WithGraphState ({}).WithIndexedPolicy (
                    {.cache_policy = {IrGraph::CachePolicy ().WithPolicyIdentifier (
                         "test_policy_2")},
                     .processor_index = 1});
                IrGraphProcessor::BoxedBuffer result_different {};
                process_result_pool.CacheResult (key_different, result_different);

                REQUIRE (result.impl () != result_different.impl ());

                THEN ("both values can be retrieved with their respective keys")
                {
                    REQUIRE (process_result_pool.GetResult (key)->impl () == result.impl ());
                    REQUIRE (process_result_pool.GetResult (key_different)->impl () ==
                             result_different.impl ());
                }
            }
        }
    }

    GIVEN ("a pool with cached results")
    {
        ProcessResultPool process_result_pool;

        auto key_1 = GraphStateKey ().WithGraphState ({}).WithIndexedPolicy (
            {.cache_policy = {IrGraph::CachePolicy ().WithPolicyIdentifier ("test_policy_1")},
             .processor_index = 0});
        IrGraphProcessor::BoxedBuffer result_1 {};
        process_result_pool.CacheResult (key_1, result_1);

        THEN ("the pool size is 1")
        {
            REQUIRE (process_result_pool.GetPoolSize () == 1);
        }

        WHEN ("the same key is added with the same result")
        {
            process_result_pool.CacheResult (key_1, result_1);

            THEN ("the pool size is 1")
            {
                REQUIRE (process_result_pool.GetPoolSize () == 1);
            }

            THEN ("key_1 can be retrieved")
            {
                REQUIRE (process_result_pool.GetResult (key_1)->impl () == result_1.impl ());
            }
        }

        WHEN ("the same key is added with a different result")
        {
            IrGraphProcessor::BoxedBuffer result_2 {};
            process_result_pool.CacheResult (key_1, result_2);

            THEN ("the pool size is 1")
            {
                REQUIRE (process_result_pool.GetPoolSize () == 1);
            }

            THEN ("key_1 retrieves result_2")
            {
                REQUIRE (process_result_pool.GetResult (key_1)->impl () == result_2.impl ());
            }
        }
    }
}

SCENARIO ("pool can clear unused values", "[ProcessResultPool]")
{
    GIVEN ("an empty pool")
    {
        ProcessResultPool process_result_pool;

        WHEN ("unused results are cleared")
        {
            process_result_pool.RemoveUnusedKeys ({});

            THEN ("the size of the pool is 0")
            {
                REQUIRE (process_result_pool.GetPoolSize () == 0);
            }
        }
    }

    GIVEN ("a pool with cached results")
    {
        ProcessResultPool process_result_pool;

        auto key_1 = GraphStateKey ().WithGraphState ({}).WithIndexedPolicy (
            {.cache_policy = {IrGraph::CachePolicy ().WithPolicyIdentifier ("test_policy_1")},
             .processor_index = 0});
        IrGraphProcessor::BoxedBuffer result_1 {};
        process_result_pool.CacheResult (key_1, result_1);

        auto key_2 = GraphStateKey ().WithGraphState ({}).WithIndexedPolicy (
            {.cache_policy = {IrGraph::CachePolicy ().WithPolicyIdentifier ("test_policy_2")},
             .processor_index = 0});
        IrGraphProcessor::BoxedBuffer result_2 {};
        process_result_pool.CacheResult (key_2, result_2);

        WHEN ("unused results are cleared and no keys are preserved")
        {
            process_result_pool.RemoveUnusedKeys ({});

            THEN ("the pool size is 0")
            {
                REQUIRE (process_result_pool.GetPoolSize () == 0);
            }
            THEN ("previously cached values cannot be retrieved")
            {
                REQUIRE (process_result_pool.GetResult (key_1) == std::nullopt);
                REQUIRE (process_result_pool.GetResult (key_2) == std::nullopt);
            }
        }

        WHEN ("unused results are cleared preserving key_2")
        {
            process_result_pool.RemoveUnusedKeys ({key_2});

            THEN ("the pool size is 1")
            {
                REQUIRE (process_result_pool.GetPoolSize () == 1);
            }

            THEN ("key_2 can be retrieved")
            {
                REQUIRE (process_result_pool.GetResult (key_2)->impl () == result_2.impl ());
            }

            THEN ("key_1 cannot be retrieved")
            {
                REQUIRE (process_result_pool.GetResult (key_1) == std::nullopt);
            }
        }

        WHEN ("unused results are cleared preserving a key not in the pool")
        {
            process_result_pool.RemoveUnusedKeys ({GraphStateKey ()});

            THEN ("the pool size is 0")
            {
                REQUIRE (process_result_pool.GetPoolSize () == 0);
            }
        }
    }
}