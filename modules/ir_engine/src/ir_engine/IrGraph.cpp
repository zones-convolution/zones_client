#include "IrGraph.h"

std::vector<GraphStateKey> IrGraph::GetKeysForState (const IrGraphState & state) const
{
    std::vector<GraphStateKey> keys;
    auto last_key = GraphStateKey ().WithGraphState (state);

    for (auto processor_index = 0; processor_index < processors_.size (); ++processor_index)
    {
        const auto [policy, _] = processors_ [processor_index];
        last_key = last_key.WithIndexedPolicy (
            {.cache_policy = policy, .processor_index = processor_index});
        keys.push_back (last_key);
    }

    return keys;
}

IrGraph IrGraph::WithProcessor (const ProcessorWithCachePolicy & processor) const
{
    auto graph = *this;
    graph.processors_ = graph.processors_.push_back (processor);
    return graph;
}

void IrGraph::Process (juce::dsp::ProcessContextNonReplacing<float> & process_context,
                       const IrGraphState & state)
{
    auto keys = GetKeysForState (state);

    for (auto processor_index = 0; processor_index < processors_.size (); ++processor_index)
    {
        auto result = pool [keys [processor_index]];
    }
}
