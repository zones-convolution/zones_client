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

std::optional<ProcessResultPool::SharedBuffer>
IrGraph::Process (const IrGraphState & state, ProcessResultPool & process_result_pool)
{
    auto keys = GetKeysForState (state);
    std::optional<ProcessResultPool::SharedBuffer> last_result;

    for (auto processor_index = 0; processor_index < processors_.size (); ++processor_index)
    {
        auto key = keys [processor_index];
        auto result = process_result_pool.GetResult (key);

        if (result.has_value ())
        {
            last_result = result;
            continue;
        }

        auto & [_, processor] = processors_ [processor_index];
        ProcessResultPool::SharedBuffer process_result =
            std::make_shared<juce::AudioBuffer<float>> ();
        juce::dsp::AudioBlock<float> process_result_block {*process_result.get ()};
        juce::dsp::ProcessContextNonReplacing<float> process_result_context {
            last_result.has_value () ? juce::dsp::AudioBlock<float> {*last_result.value ().get ()}
                                     : juce::dsp::AudioBlock<float> {},
            process_result_block};
        processor.lock ()->Process (process_result_context, state);
        process_result_pool.CacheResult (key, process_result);
        last_result = process_result;
    }

    return last_result;
}