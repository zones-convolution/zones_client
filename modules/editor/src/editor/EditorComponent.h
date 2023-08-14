#pragma once

#include "ir_engine/IrEngine.h"
#include "model/ParameterAction.h"
#include "panels/IOPanel.h"
#include "spectrogram_visualiser/component/Graph3DComponent.h"
#include "zones_look_and_feel/LookAndFeel.h"
#include "zones_look_and_feel/components/PanelComponent.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <lager/context.hpp>

class EditorComponent
    : public juce::Component
    , public IrEngine::Listener
{
public:
    explicit EditorComponent (
        lager::context<RealtimeParameterAction> & realtime_parameter_context,
        lager::context<IrEngineParameterAction> & ir_engine_parameter_context);
    void resized () override;

    void RenderFinished (IrGraphState state, IrGraphProcessor::BoxedBuffer render_result) override;

private:
    lager::context<RealtimeParameterAction> realtime_parameter_context_;
    lager::context<IrEngineParameterAction> ir_engine_parameter_context_;

    juce::Label room_size_label_;
    juce::Slider room_size_slider_ {juce::Slider::SliderStyle::LinearHorizontal,
                                    juce::Slider::TextEntryBoxPosition::NoTextBox};
    juce::Label reverb_time_label_;
    juce::Slider reverb_time_slider_ {juce::Slider::SliderStyle::LinearHorizontal,
                                      juce::Slider::TextEntryBoxPosition::NoTextBox};

    IOPanel io_;
    PanelComponent io_panel_ {io_};
};
