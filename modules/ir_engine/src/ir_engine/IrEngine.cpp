
#include "IrEngine.h"
IrEngine::IrEngine ()
    : thread_pool_ ()
{
}
void IrEngine::RenderState (const IrGraphState & state)
{
    thread_pool_.removeAllJobs (true, 0);

    auto render_job = new Job (ir_graph_, state);
    thread_pool_.addJob (render_job, true);
}

IrEngine::Job::Job (const IrGraph & ir_graph, const IrGraphState & state)
    : juce::ThreadPoolJob ("ir_engine_job")
    , ir_graph_ (ir_graph)
    , state_ (state)
{
}

juce::ThreadPoolJob::JobStatus IrEngine::Job::runJob ()
{
    auto keys = ir_graph_.GetKeysForState (state_);
    ir_graph_.

        return jobHasFinished;
}
