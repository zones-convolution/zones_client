#include "VisualiserController.h"

void VisualiserController::RenderFinished (IrGraphState state,
                                           IrGraphProcessor::BoxedBuffer render_result)
{
    std::lock_guard render_guard {render_mutex_};
    last_render_result_ = render_result;
    thread_pool_.removeAllJobs (true, 0);
    thread_pool_.addJob (
        new RenderVisualiserJob (render_result,
                                 [&] (BoxedBuffer frequency_data)
                                 {
                                     std::lock_guard callback_guard {render_mutex_};
                                     frequency_data_ = frequency_data;
                                     juce::MessageManager::callAsync (
                                         [&, frequency_data]
                                         { OnVisualiserRendered (frequency_data); });
                                 }),
        true);
}

VisualiserController::BoxedBuffer VisualiserController::GetVisualiserRender ()
{
    std::lock_guard render_guard {render_mutex_};
    return frequency_data_;
}
