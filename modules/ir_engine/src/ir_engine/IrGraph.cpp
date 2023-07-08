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

std::optional<IrGraphProcessor::BoxedBuffer>
IrGraph::Process (const IrGraphState & state,
                  ProcessResultPool & process_result_pool,
                  AbortCallback abort_callback)
{
    auto keys = GetKeysForState (state);
    std::optional<IrGraphProcessor::BoxedBuffer> last_result;

    for (auto processor_index = 0; processor_index < processors_.size (); ++processor_index)
    {
        if (abort_callback ())
            return std::nullopt;

        auto key = keys [processor_index];
        auto result = process_result_pool.GetResult (key);

        if (result.has_value ())
        {
            last_result = result;
            continue;
        }

        auto & [_, processor] = processors_ [processor_index];
        juce::AudioBuffer<float> process_result;

        auto empty_buffer = IrGraphProcessor::BoxedBuffer (juce::AudioBuffer<float> {});
        processor.lock ()->Process (
            last_result.has_value () ? last_result.value () : empty_buffer, process_result, state);

        auto boxed_result = IrGraphProcessor::BoxedBuffer (std::move (process_result));
        process_result_pool.CacheResult (key, boxed_result);
        last_result = boxed_result;
    }

    return last_result;
}