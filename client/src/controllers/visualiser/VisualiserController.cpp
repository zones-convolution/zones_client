#include "VisualiserController.h"

void VisualiserController::RenderFinished (IrGraphState state,
                                           IrGraphProcessor::BoxedBuffer render_result)
{
    std::lock_guard render_guard {render_mutex_};

    last_rendered_state_ = state;
    last_render_result_ = render_result;
    auto base_ir_buffer_samples = state.base_ir_buffer->getNumSamples ();

    int interpolated_base_ir_buffer_samples;
    if (state.sample_rate > 0.f)
    {
        interpolated_base_ir_buffer_samples =
            static_cast<int> (std::ceil ((state.sample_rate / state.base_ir_sample_rate) *
                                         static_cast<float> (base_ir_buffer_samples)));
    }
    else
    {
        interpolated_base_ir_buffer_samples = base_ir_buffer_samples;
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

VisualiserMetadata VisualiserController::GetMetadata ()
{
    std::lock_guard render_guard {render_mutex_};

    auto base_sample_rate = last_rendered_state_.base_ir_sample_rate;
    auto ir_engine_sample_rate = last_rendered_state_.sample_rate;

    auto metadata_sample_rate =
        ir_engine_sample_rate == 0.0 ? base_sample_rate : ir_engine_sample_rate;

    auto base_ir_length_samples =
        static_cast<int> (std::ceil (last_rendered_state_.base_ir_buffer->getNumSamples () *
                                     (metadata_sample_rate / base_sample_rate)));

    return {.sample_rate = metadata_sample_rate, .base_ir_length_samples = base_ir_length_samples};
}
