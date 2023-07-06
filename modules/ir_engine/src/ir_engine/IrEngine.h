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
        using RenderFinishedCallback = std::function<void (IrGraphProcessor::BoxedBuffer)>;
        Job (const IrGraph & ir_graph,
             const IrGraphState & state,
             ProcessResultPool & result_pool,
             RenderFinishedCallback callback);

    private:
        JobStatus runJob () override;

        RenderFinishedCallback callback_;
        ProcessResultPool & result_pool_;
        IrGraph ir_graph_;
        IrGraphState state_;
    };

    void CleanPool ();

    static constexpr int kJobTimeout = 200;
    static constexpr int kMaxNumberOfJobsSinceLastClean = 4;
    int jobs_since_last_clean_ = 0;

    IrGraphState last_rendered_state_;
    ProcessResultPool result_pool_;
    juce::ThreadPool thread_pool_;

    std::shared_ptr<IrGraphProcessor> test_processor_ = std::make_shared<TestProcessor> ();
    IrGraph ir_graph_ = IrGraph ().WithProcessor (
        {IrGraph::CachePolicy ().WithPolicyIdentifier ("test_processor"), test_processor_});
};
