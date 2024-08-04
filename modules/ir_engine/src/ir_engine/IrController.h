#pragma once

#include "IrEngine.h"
#include "format/IrData.h"
#include "model/ParameterTree.h"

class IrController
    : public juce::AudioProcessorParameter::Listener
    , private juce::Timer
{
public:
    IrController (IrEngine & ir_engine, ParameterTree & parameter_tree);
    ~IrController () override = default;
    void parameterValueChanged (int parameter_index, float new_value) override;
    void parameterGestureChanged (int parameterIndex, bool gestureIsStarting) override;
    void timerCallback () override;

    void LoadIr (const IrSelection & ir_selection);
    IrGraphState GetCurrentGraphState ();

private:
    void UpdateParametersFromTree ();
    void PerformRender ();

    static constexpr auto kDebounceTimeMs = 10.f;

    IrEngine & ir_engine_;

    std::mutex current_graph_state_mutex_;
    IrGraphState current_graph_state_ {};

    ParameterTree & parameter_tree_;
};
