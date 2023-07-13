#include "IrEngine.h"

#include <juce_events/juce_events.h>
#include <utility>

IrEngine::IrEngine ()
    : thread_pool_ ()
{
}

void IrEngine::RenderState (const IrGraphState & state, RenderFinishedCallback callback)
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

    auto render_job = new Job (ir_graph_,
                               state,
                               result_pool_,
                               [&, callback] (IrGraphProcessor::BoxedBuffer process_result)
                               {
                                   CleanPool ();
                                   juce::MessageManager::callAsync ([process_result, callback] ()
                                                                    { callback (process_result); });
                               });

    thread_pool_.addJob (render_job, true);
}

void IrEngine::CleanPool ()
{
    result_pool_.RemoveUnusedKeys (ir_graph_.GetKeysForState (last_rendered_state_));
    jobs_since_last_clean_ = 0;
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
