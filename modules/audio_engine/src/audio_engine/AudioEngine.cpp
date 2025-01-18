#include "AudioEngine.h"

AudioEngine::AudioEngine (CommandQueue::VisitorQueue & command_queue,
                          NotificationQueue::VisitorQueue & notification_queue,
                          zones::ConvolutionEngine & convolution_engine,
                          PlayerController & player_controller,
                          const juce::AudioProcessor & processor)
    : player_controller_ (player_controller)
    , processor_ (processor)
    , command_queue_ (command_queue)
    , notification_queue_ (notification_queue)
    , convolution_engine_ (convolution_engine)
{
    startTimer (kServiceIntervalMs);
}

zones::Convolver::ConvolverSpec
AudioEngine::CreateConvolverSpecForState (const IrGraphState & ir_graph_state) const
{
    auto fade_strategy = ir_graph_state.base_ir == last_graph_state_.base_ir
                             ? zones::Convolver::FadeStrategy::kCrossfade
                             : zones::Convolver::FadeStrategy::kInOut;

    auto block_size = ir_graph_state.convolver_block_size;

    switch (ir_graph_state.target_format)
    {
        case TargetFormat::kMono:
            return zones::Convolver::ConvolverSpec {.input_routing = {0},
                                                    .output_routing = {0},
                                                    .fade_strategy = fade_strategy,
                                                    .internal_block_size = block_size};
        case TargetFormat::kStereo:
            return zones::Convolver::ConvolverSpec {.input_routing = {0, 1},
                                                    .output_routing = {0, 1},
                                                    .fade_strategy = fade_strategy,
                                                    .internal_block_size = block_size};
        case TargetFormat::kTrueStereo:
            return zones::Convolver::ConvolverSpec {.input_routing = {0, 0, 1, 1},
                                                    .output_routing = {0, 1, 0, 1},
                                                    .fade_strategy = fade_strategy,
                                                    .internal_block_size = block_size};
        case TargetFormat::kFoa:
        case TargetFormat::kQuadraphonic:
            return zones::Convolver::ConvolverSpec {.input_routing = {0, 1, 2, 3},
                                                    .output_routing = {0, 1, 2, 3},
                                                    .fade_strategy = fade_strategy,
                                                    .internal_block_size = block_size};
    }
}

void AudioEngine::RenderFinished (IrGraphState state, IrGraphProcessor::BoxedBuffer render_result)
{
    if (render_result->getNumChannels () == 0 || render_result->getNumSamples () == 0)
        return;

    auto output_channel_set = processor_.getBusesLayout ().getMainOutputChannelSet ();
    if (IsTargetSupported (output_channel_set, state.target_format))
        convolution_engine_.LoadIR (*render_result, CreateConvolverSpecForState (state));
    else
        jassertfalse;

    last_graph_state_ = state;
}

void AudioEngine::operator() (const Player::PlayerState & player_state_notification)
{
    player_controller_.ReceivedPlayerStateNotification (player_state_notification);
}

void AudioEngine::timerCallback ()
{
    notification_queue_.Service ();
}
