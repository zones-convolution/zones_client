#pragma once

#include "CommandQueue.h"
#include "NotificationQueue.h"
#include "format/IrData.h"
#include "ir_engine/IrEngine.h"
#include "model/ParameterTree.h"
#include "player/PlayerController.h"
#include "zones_convolver/zones_convolver.h"

#include <juce_audio_processors/juce_audio_processors.h>

class AudioEngine
    : public IrEngine::Listener
    , public NotificationQueue::Visitor
    , public juce::Timer
{
public:
    explicit AudioEngine (CommandQueue::VisitorQueue & command_queue,
                          NotificationQueue::VisitorQueue & notification_queue,
                          zones::ConvolutionEngine & convolution_engine,
                          PlayerController & player_controller,
                          const juce::AudioProcessor & processor);
    void RenderFinished (IrGraphState state, IrGraphProcessor::BoxedBuffer render_result) override;
    ~AudioEngine () override = default;
    void operator() (const Player::PlayerState & player_state_notification) override;
    void timerCallback () override;

private:
    static constexpr auto kServiceIntervalMs = 100;

    PlayerController & player_controller_;

    [[nodiscard]] zones::Convolver::ConvolverSpec
    CreateConvolverSpecForState (const IrGraphState & ir_graph_state) const;

    const juce::AudioProcessor & processor_;
    CommandQueue::VisitorQueue & command_queue_;
    NotificationQueue::VisitorQueue & notification_queue_;
    zones::ConvolutionEngine & convolution_engine_;
    IrGraphState last_graph_state_ {};
};
