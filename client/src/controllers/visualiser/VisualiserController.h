#pragma once

#include "Spectrogram.h"
#include "ir_engine/IrController.h"
#include "ir_engine/IrEngine.h"
#include "juce_gui_extra/juce_gui_extra.h"
#include "rocket.hpp"

#include <immer/box.hpp>

struct VisualiserMetadata
{
    double sample_rate = 48000.0;
    int base_ir_length_samples = 48000;
};

class VisualiserController : public IrEngine::Listener
{
public:
    void RenderFinished (IrGraphState state, IrGraphProcessor::BoxedBuffer render_result) override;

    class RenderVisualiserJob : public juce::ThreadPoolJob
    {
    public:
        struct Parameters
        {
            IrGraphProcessor::BoxedBuffer result_buffer;
            int base_num_samples;
            double base_sample_rate;
        };

        RenderVisualiserJob (Parameters parameters,
                             std::function<void (Spectrogram::BoxedUint8Buffer)> render_callback)
            : juce::ThreadPoolJob ("RenderVisualiserJob")
            , parameters_ (parameters)
            , render_callback_ (render_callback)
        {
        }

        JobStatus runJob () override
        {
            auto render_result =
                Spectrogram::CreateNormalisedSpectrogramData (parameters_.result_buffer,
                                                              parameters_.base_num_samples,
                                                              parameters_.base_sample_rate);

            if (! shouldExit ())
                render_callback_ (render_result);

            return jobHasFinished;
        }

    private:
        Parameters parameters_;
        std::function<void (Spectrogram::BoxedUint8Buffer)> render_callback_;
    };

    rocket::signal<void ()> OnVisualiserRendered;
    std::optional<Spectrogram::BoxedUint8Buffer> GetVisualiserRender ();

    VisualiserMetadata GetMetadata ();

private:
    std::mutex render_mutex_;
    std::optional<Spectrogram::BoxedBuffer> last_render_result_ = std::nullopt;
    juce::ThreadPool thread_pool_;
    std::optional<Spectrogram::BoxedUint8Buffer> frequency_data_ = std::nullopt;

    IrGraphState last_rendered_state_;
};