#pragma once
#include "CommandQueue.h"
#include "ir_engine/IrEngine.h"
#include "ir_repository/project/ProjectIrLoadController.h"
#include "model/Model.h"
#include "model/ParameterTree.h"

class AudioEngine
    : public IrEngine::Listener
    , public juce::AudioProcessorValueTreeState::Listener
{
public:
    explicit AudioEngine (CommandQueue::VisitorQueue & command_queue,
                          juce::AudioProcessorValueTreeState & parameter_tree);
    void LoadIr (const IrData & ir_data);
    void RenderFinished (IrGraphState state, IrGraphProcessor::BoxedBuffer render_result) override;
    ~AudioEngine () override = default;
    void parameterChanged (const juce::String & parameterID, float newValue) override;

private:
    CommandQueue::VisitorQueue & command_queue_;
    juce::AudioProcessorValueTreeState & parameter_tree_;
};
