#pragma once

#include "IrGraph.h"
#include "juce_core/juce_core.h"
#include "processors/BaseIrProcessor.h"
#include "processors/RoomSizeProcessor.h"

class IrEngine
{
public:
    IrEngine ();

    using RenderFinishedCallback = std::function<void (IrGraphProcessor::BoxedBuffer)>;
    void RenderState (const IrGraphState & state, RenderFinishedCallback callback);

    class Job : public juce::ThreadPoolJob
    {
    public:
        Job (const IrGraph & ir_graph,
             const IrGraphState & state,
             ProcessResultPool & result_pool,
             RenderFinishedCallback callback);

        JobStatus runJob () override;

    private:
        RenderFinishedCallback callback_;
        ProcessResultPool & result_pool_;
        IrGraph ir_graph_;
        IrGraphState state_;
    };

private:
    void CleanPool ();

    static constexpr int kJobTimeout = 200;
    static constexpr int kMaxNumberOfJobsSinceLastClean = 4;
    int jobs_since_last_clean_ = 0;

    IrGraphState last_rendered_state_;
    ProcessResultPool result_pool_;
    juce::ThreadPool thread_pool_;

    std::shared_ptr<BaseIrProcessor> base_ir_processor_ = std::make_shared<BaseIrProcessor> ();
    std::shared_ptr<RoomSizeProcessor> room_size_processor_ =
        std::make_shared<RoomSizeProcessor> ();

    IrGraph ir_graph_ =
        IrGraph ()
            .WithProcessor ({IrGraph::CachePolicy ()
                                 .WithPolicyIdentifier ("base_ir_processor")
                                 .WithCachedHandle (&IrGraphState::CacheBaseIr),
                             base_ir_processor_})
            .WithProcessor ({IrGraph::CachePolicy ().WithPolicyIdentifier ("room_size_processor"),
                             room_size_processor_});
};
