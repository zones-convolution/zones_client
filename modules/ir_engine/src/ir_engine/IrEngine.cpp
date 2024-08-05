#include "IrEngine.h"

#include <juce_events/juce_events.h>

void IrEngine::RenderState (const IrGraphState & state)
{
    if (state.base_ir_buffer->getNumSamples () == 0) // Possibly a better way to check for this
        return;

    std::lock_guard guard {render_mutex_};

    ++jobs_since_last_clean_;
    last_rendered_state_ = state;

    last_graph_ = CreateGraphForState (state);

    if (jobs_since_last_clean_ > kMaxNumberOfJobsSinceLastClean)
    {
        thread_pool_.removeAllJobs (true, kJobTimeout);
        CleanPool ();
    }

    auto render_job = new Job (
        last_graph_,
        state,
        result_pool_,
        [&] (std::optional<IrGraphProcessor::BoxedBuffer> process_result, juce::ThreadPoolJob * job)
        {
            std::lock_guard guard {render_mutex_};

            CleanPool ();

            if (job->shouldExit ())
                return;

            auto render_it = std::find (pending_renders_.begin (), pending_renders_.end (), job);
            if (render_it == pending_renders_.end ()) // If the job doesnt exists in the queue
                return;

            for (auto itr = pending_renders_.begin (); itr != render_it;
                 ++itr) // invalidate old renders
                thread_pool_.removeJob (*itr, true, 0);

            pending_renders_.erase (pending_renders_.begin (), std::next (render_it));

            if (pending_renders_.empty ())
                is_loading_ = false;

            juce::MessageManager::callAsync (
                [&, state, process_result] ()
                {
                    OnLoadingUpdated ();

                    if (process_result.has_value ())
                    {
                        listeners_.call (
                            [state, process_result] (Listener & listener)
                            { listener.RenderFinished (state, process_result.value ()); });
                    }
                });
        });

    pending_renders_.emplace_back (render_job);
    thread_pool_.addJob (render_job, true);
    thread_pool_.moveJobToFront (render_job);

    is_loading_ = true;
    juce::MessageManager::callAsync ([&] { OnLoadingUpdated (); });
}

void IrEngine::CleanPool ()
{
    result_pool_.RemoveUnusedKeys (last_graph_.GetKeysForState (last_rendered_state_));
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
        case TargetFormat::kStereo:
        case TargetFormat::kTrueStereo:
            return ir_graph
                .WithProcessor ({IrGraph::CachePolicy ()
                                     .WithPolicyIdentifier ("reverb_time_processor")
                                     .WithCachedHandle (&IrGraphState::CacheReverbTime),
                                 reverb_time_processor_})
                .WithProcessor ({IrGraph::CachePolicy ()
                                     .WithPolicyIdentifier ("resampler_processor")
                                     .WithCachedHandle (&IrGraphState::CacheResamplerRatio),
                                 resampler_processor_})
                .WithProcessor ({IrGraph::CachePolicy ()
                                     .WithPolicyIdentifier ("time_processor")
                                     .WithCachedHandle (&IrGraphState::CachePredelay)
                                     .WithCachedHandle (&IrGraphState::CacheTrim)
                                     .WithCachedHandle (&IrGraphState::CacheAttack),
                                 predelay_processor_});
        default:
            return ir_graph;
    }
}

IrEngine::IrEngine (juce::ThreadPool & thread_pool)
    : thread_pool_ (thread_pool)
{
}

bool IrEngine::IsLoading () const
{
    return is_loading_;
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
    callback_ (process_result, this);
    return jobHasFinished;
}
