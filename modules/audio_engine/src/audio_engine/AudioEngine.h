#pragma once

#include "CommandQueue.h"
#include "NotificationQueue.h"
#include "format/IrData.h"
#include "ir_engine/IrEngine.h"
#include "model/Model.h"
#include "model/ParameterTree.h"
#include "player/PlayerController.h"
#include "zones_convolver/zones_convolver.h"

#include <juce_audio_processors/juce_audio_processors.h>

class AudioEngine
    : public IrEngine::Listener
    , public juce::AudioProcessorValueTreeState::Listener
    , public NotificationQueue::Visitor
{
public:
    explicit AudioEngine (CommandQueue::VisitorQueue & command_queue,
                          juce::AudioProcessorValueTreeState & parameter_tree,
                          zones::ConvolutionEngine & convolution_engine,
                          const juce::AudioProcessor & processor);
    void RenderFinished (IrGraphState state, IrGraphProcessor::BoxedBuffer render_result) override;
    ~AudioEngine () override = default;
    void parameterChanged (const juce::String & parameterID, float newValue) override;
    void operator() (
        const NotificationQueue::PlayerStateNotification & player_state_notification) override;

private:
    [[nodiscard]] static zones::Convolver::ConvolverSpec
    CreateConvolverSpecForState (const IrGraphState & ir_graph_state);

    const juce::AudioProcessor & processor_;
    CommandQueue::VisitorQueue & command_queue_;
    juce::AudioProcessorValueTreeState & parameter_tree_;
    zones::ConvolutionEngine & convolution_engine_;

    PlayerController player_controller_;
};
