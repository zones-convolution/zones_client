#pragma once

#include "IrGraphCachePolicy.h"
#include "IrGraphProcessor.h"

#include <immer/box.hpp>
#include <immer/flex_vector.hpp>

struct IndexedGraphPolicy
{
    IrGraphCachePolicy<IrGraphState> cache_policy;
    int processor_index;
};

struct GraphStateKey
{
    bool operator== (const GraphStateKey & other) const;

    [[nodiscard]] GraphStateKey WithGraphState (const IrGraphState & state) const;
    [[nodiscard]] GraphStateKey WithIndexedPolicy (const IndexedGraphPolicy & policy) const;

    immer::box<IrGraphState> graph_state;
    immer::flex_vector<IndexedGraphPolicy> policies;
};

struct GraphStateKeyHashFn
{
    std::size_t operator() (const GraphStateKey & p) const;
};