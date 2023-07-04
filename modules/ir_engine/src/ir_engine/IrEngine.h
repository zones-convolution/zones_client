#pragma once
#include "IrGraph.h"
#include "juce_core/juce_core.h"
#include "processors/TestProcessor.h"

class IrEngine
{
public:
    IrEngine ();

    void RenderState (const IrGraphState & state);

private:
    class Job : public juce::ThreadPoolJob
    {
    public:
        Job (const IrGraph & ir_graph, const IrGraphState & state);

    private:
        JobStatus runJob () override;

        IrGraph ir_graph_;
        IrGraphState state_;
    };

    juce::ThreadPool thread_pool_;

    std::shared_ptr<IrGraphProcessor> test_processor_ = std::make_shared<TestProcessor> ();
    IrGraph ir_graph_ = IrGraph ().WithProcessor (
        {IrGraph::CachePolicy ().WithPolicyIdentifier ("test_processor"), test_processor_});
};
