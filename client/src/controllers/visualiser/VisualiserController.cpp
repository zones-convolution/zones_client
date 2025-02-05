#include "VisualiserController.h"

void VisualiserController::RenderFinished (IrGraphState state,
                                           IrGraphProcessor::BoxedBuffer render_result)
{
    std::lock_guard render_guard {render_mutex_};
    last_render_result_ = render_result;
    auto base_ir_buffer_samples = state.base_ir_buffer->getNumSamples ();

    int interpolated_base_ir_buffer_samples;
    if (state.sample_rate > 0.f)
    {
        interpolated_base_ir_buffer_samples =
            static_cast<int> (std::ceil ((state.sample_rate / state.base_ir_sample_rate) *
                                         static_cast<float> (base_ir_buffer_samples)));

        state_sample_rate = state.sample_rate;
    }
    else
    {
        interpolated_base_ir_buffer_samples = base_ir_buffer_samples;
        state_sample_rate = state.base_ir_sample_rate;
    }
    
    thread_pool_.removeAllJobs (true, 0);
    thread_pool_.addJob (
        new RenderVisualiserJob (
            RenderVisualiserJob::Parameters {.result_buffer = render_result,
                                             .base_num_samples =
                                                 interpolated_base_ir_buffer_samples,
                                             .base_sample_rate = state.sample_rate},
            [&] (Spectrogram::BoxedUint8Buffer frequency_data)
            {
                std::lock_guard callback_guard {render_mutex_};
                frequency_data_ = frequency_data;
                juce::MessageManager::callAsync ([&, frequency_data] { OnVisualiserRendered (); });
            }),
        true);
}

std::optional<Spectrogram::BoxedUint8Buffer> VisualiserController::GetVisualiserRender ()
{
    std::lock_guard render_guard {render_mutex_};
    return frequency_data_;
}

double VisualiserController::GetStateSampleRate ()
{
    return state_sample_rate;
}
