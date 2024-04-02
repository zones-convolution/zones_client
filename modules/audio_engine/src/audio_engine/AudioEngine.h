#pragma once
#include "CommandQueue.h"
#include "ir_engine/IrEngine.h"
#include "model/Model.h"
#include "model/ParameterTree.h"
#include "zones_convolver/zones_convolver.h"

class AudioEngine
    : public IrEngine::Listener
    , public juce::AudioProcessorValueTreeState::Listener
{
public:
    explicit AudioEngine (CommandQueue::VisitorQueue & command_queue,
                          juce::AudioProcessorValueTreeState & parameter_tree,
                          zones::ConvolutionEngine & convolution_engine);
    void RenderFinished (IrGraphState state, IrGraphProcessor::BoxedBuffer render_result) override;
    ~AudioEngine () override = default;
    void parameterChanged (const juce::String & parameterID, float newValue) override;

private:
    CommandQueue::VisitorQueue & command_queue_;
    juce::AudioProcessorValueTreeState & parameter_tree_;
    zones::ConvolutionEngine & convolution_engine_;
};
