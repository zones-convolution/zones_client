#include "IrEngine.h"

#include <juce_events/juce_events.h>

IrEngine::IrEngine (juce::ThreadPool & thread_pool)
    : thread_pool_ (thread_pool)
{
}

void IrEngine::RenderState (const IrGraphState & state)
{
    ++jobs_since_last_clean_;
    last_rendered_state_ = state;

    if (jobs_since_last_clean_ < kMaxNumberOfJobsSinceLastClean)
    {
        thread_pool_.removeAllJobs (true, 0);
    }
    else
    {
        thread_pool_.removeAllJobs (true, kJobTimeout);
        CleanPool ();
    }

    auto render_job =
        new Job (ir_graph_,
                 state,
                 result_pool_,
                 [&] (IrGraphProcessor::BoxedBuffer process_result)
                 {
                     CleanPool ();
                     juce::MessageManager::callAsync (
                         [&, state, process_result] ()
                         {
                             listeners_.call ([state, process_result] (Listener & listener)
                                              { listener.RenderFinished (state, process_result); });
                         });
                 });

    thread_pool_.addJob (render_job, true);
}

void IrEngine::CleanPool ()
{
    result_pool_.RemoveUnusedKeys (ir_graph_.GetKeysForState (last_rendered_state_));
    jobs_since_last_clean_ = 0;
}

juce::ListenerList<IrEngine::Listener> & IrEngine::GetListeners ()
{
    return listeners_;
}

IrEngine::Job::Job (const IrGraph & ir_graph,
                    const IrGraphState & state,
                    ProcessResultPool & result_pool,
                    RenderFinishedCallback callback)
    : juce::ThreadPoolJob ("ir_engine_job")
    , ir_graph_ (ir_graph)
    , state_ (state)
    , result_pool_ (result_pool)
    , callback_ (callback)
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
