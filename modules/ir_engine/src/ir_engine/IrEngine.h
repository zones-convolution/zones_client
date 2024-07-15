#pragma once

#include "IrGraph.h"
#include "juce_core/juce_core.h"
#include "processors/BaseIrProcessor.h"
#include "processors/PreDelayProcessor.h"
#include "processors/ResamplerProcessor.h"
#include "processors/ReverbTimeProcessor.h"
#include "processors/RoomSizeProcessor.h"

#include <rocket.hpp>

class IrEngine
{
public:
    struct Listener
    {
        virtual void RenderFinished (IrGraphState state,
                                     IrGraphProcessor::BoxedBuffer render_result) = 0;
    };

    IrEngine (juce::ThreadPool & thread_pool);

    using RenderFinishedCallback =
        std::function<void (std::optional<IrGraphProcessor::BoxedBuffer>, juce::ThreadPoolJob *)>;
    void RenderState (const IrGraphState & state);

    juce::ListenerList<Listener> & GetListeners ();

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

    [[nodiscard]] bool IsLoading () const;
    rocket::signal<void ()> OnLoadingUpdated;

private:
    [[nodiscard]] IrGraph CreateGraphForState (const IrGraphState & ir_graph_state) const;
    void CleanPool ();

    static constexpr int kJobTimeout = 200;
    static constexpr int kMaxNumberOfJobsSinceLastClean = 4;
    int jobs_since_last_clean_ = 0;

    juce::ListenerList<Listener> listeners_;

    IrGraph last_graph_;
    IrGraphState last_rendered_state_;
    ProcessResultPool result_pool_;
    juce::ThreadPool & thread_pool_;

    std::shared_ptr<BaseIrProcessor> base_ir_processor_ = std::make_shared<BaseIrProcessor> ();
    std::shared_ptr<RoomSizeProcessor> room_size_processor_ =
        std::make_shared<RoomSizeProcessor> ();
    std::shared_ptr<ReverbTimeProcessor> reverb_time_processor_ =
        std::make_shared<ReverbTimeProcessor> ();
    std::shared_ptr<ResamplerProcessor> resampler_processor_ =
        std::make_shared<ResamplerProcessor> ();
    std::shared_ptr<PreDelayProcessor> predelay_processor_ = std::make_shared<PreDelayProcessor> ();

    std::mutex render_mutex_;
    std::vector<juce::ThreadPoolJob *> pending_renders_;

    std::atomic<bool> is_loading_ = false;
};
