#pragma once

#include "IrEngine.h"
#include "format/IrData.h"
#include "model/ParameterTree.h"

class IrController
    : public juce::AudioProcessorValueTreeState::Listener
    , private juce::Timer
{
public:
    IrController (IrEngine & ir_engine, juce::AudioProcessorValueTreeState & parameter_tree);
    ~IrController () override = default;
    void parameterChanged (const juce::String & parameter_id, float new_value) override;
    void timerCallback () override;

    void LoadIr (const IrSelection & ir_selection);

private:
    void UpdateParametersFromTree ();
    void PerformRender ();

    static constexpr auto kDebounceTimeMs = 160.f;

    IrEngine & ir_engine_;

    std::mutex current_graph_state_mutex_;
    IrGraphState current_graph_state_ {};

    IrGraphProcessor::BoxedBuffer last_render_result_;
    juce::AudioProcessorValueTreeState & parameter_tree_;
};
