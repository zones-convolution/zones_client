#include "IrEngine.h"

#include <juce_events/juce_events.h>

void IrEngine::RenderState (const IrGraphState & state)
{
    ++jobs_since_last_clean_;
    last_rendered_state_ = state;

    auto ir_graph = CreateGraphForState (state);

    if (jobs_since_last_clean_ < kMaxNumberOfJobsSinceLastClean)
    {
        thread_pool_.removeAllJobs (true, 0);
    }
    else
    {
        thread_pool_.removeAllJobs (true, kJobTimeout);
        CleanPool (ir_graph);
    }

    auto render_job =
        new Job (ir_graph,
                 state,
                 result_pool_,
                 [&, ir_graph] (IrGraphProcessor::BoxedBuffer process_result)
                 {
                     CleanPool (ir_graph);
                     juce::MessageManager::callAsync (
                         [&, state, process_result] ()
                         {
                             listeners_.call ([state, process_result] (Listener & listener)
                                              { listener.RenderFinished (state, process_result); });
                         });
                 });

    thread_pool_.addJob (render_job, true);
}

void IrEngine::CleanPool (const IrGraph & ir_graph)
{
    result_pool_.RemoveUnusedKeys (ir_graph.GetKeysForState (last_rendered_state_));
    jobs_since_last_clean_ = 0;
}

juce::ListenerList<IrEngine::Listener> & IrEngine::GetListeners ()
{
    return listeners_;
}

IrGraph IrEngine::CreateGraphForState (const IrGraphState & ir_graph_state) const
{
    auto ir_graph =
        IrGraph ().WithProcessor ({IrGraph::CachePolicy ()
                                       .WithPolicyIdentifier ("base_ir_processor")
                                       .WithCachedHandle (&IrGraphState::CacheBaseIr)
                                       .WithCachedHandle (&IrGraphState::CacheTargetFormat),
                                   base_ir_processor_});
    switch (ir_graph_state.target_format)
    {
        case TargetFormat::kTrueStereo:
            return ir_graph
                .WithProcessor ({IrGraph::CachePolicy ()
                                     .WithPolicyIdentifier ("room_size_processor")
                                     .WithCachedHandle (&IrGraphState::CacheRoomSize),
                                 room_size_processor_})
                .WithProcessor ({IrGraph::CachePolicy ()
                                     .WithPolicyIdentifier ("reverb_time_processor")
                                     .WithCachedHandle (&IrGraphState::CacheReverbTime),
                                 reverb_time_processor_});
        default:
            return ir_graph;
    }
}

IrEngine::IrEngine (juce::ThreadPool & thread_pool)
    : thread_pool_ (thread_pool)
{
}

IrEngine::Job::Job (const IrGraph & ir_graph,
                    const IrGraphState & state,
                    ProcessResultPool & result_pool,
                    RenderFinishedCallback callback)
    : juce::ThreadPoolJob ("ir_engine_job")
    , callback_ (callback)
    , result_pool_ (result_pool)
    , ir_graph_ (ir_graph)
    , state_ (state)
{
}

juce::ThreadPoolJob::JobStatus IrEngine::Job::runJob ()
{
    auto process_result =
        ir_graph_.Process (state_, result_pool_, [&] () { return shouldExit (); });
    if (process_result.has_value ())
        callback_ (process_result.value ());

    return jobHasFinished;
}
