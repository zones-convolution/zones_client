#pragma once

#include "IrEngine.h"
#include "ir_format/IrData.h"
#include "ir_format/IrMetadata.h"
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

    void LoadIr (const IrMetadata & ir_metadata, TargetFormat target_format);

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
