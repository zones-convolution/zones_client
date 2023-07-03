#pragma once

#include "IrGraph.h"
#include "IrGraphCachePolicy.h"

struct IndexedGraphPolicy
{
    IrGraphCachePolicy<IrGraphState> cache_policy;
    int processor_index;
};

struct GraphStateKey
{
    IrGraphState graph_state;
    std::vector<IndexedGraphPolicy> policies;

    bool operator== (const GraphStateKey & other) const;
};

struct GraphStateKeyHashFn
{
    std::size_t operator() (const GraphStateKey & p) const;
};