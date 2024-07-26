#pragma once

#include "ir_engine/IrEngine.h"
#include "juce_gui_extra/juce_gui_extra.h"
#include "rocket.hpp"

#include <immer/box.hpp>

class VisualiserController : public IrEngine::Listener
{
public:
    using BoxedBuffer = immer::box<juce::AudioBuffer<float>>;

    void RenderFinished (IrGraphState state, IrGraphProcessor::BoxedBuffer render_result) override;

    class RenderVisualiserJob : public juce::ThreadPoolJob
    {
    public:
        RenderVisualiserJob (IrGraphProcessor::BoxedBuffer render_result,
                             std::function<void (BoxedBuffer)> render_callback)
            : juce::ThreadPoolJob ("RenderVisualiserJob")
            , render_result_ (render_result)
            , render_callback_ (render_callback)
        {
        }

        JobStatus runJob () override
        {
            if (! shouldExit ())
                render_callback_ ();

            return jobHasFinished;
        }

    private:
        BoxedBuffer render_result_;
        std::function<void (BoxedBuffer)> render_callback_;
    };

    rocket::signal<void (BoxedBuffer)> OnVisualiserRendered;
    BoxedBuffer GetVisualiserRender ();

private:
    std::mutex render_mutex_;
    BoxedBuffer last_render_result_;
    juce::ThreadPool thread_pool_;
    BoxedBuffer frequency_data_;
};