#pragma once

#include "IOComponent.h"
#include "IrEngineComponent.h"
#include "ir_engine/IrEngine.h"
#include "look_and_feel/components/PanelComponent.h"
#include "visualiser/component/WaterfallComponent.h"

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

class EditorComponent
    : public juce::Component
    , public IrEngine::Listener
{
public:
    explicit EditorComponent (juce::AudioProcessorValueTreeState & parameter_tree,
                              juce::ThreadPool & thread_pool,
                              PlayerController & player_controller);
    void resized () override;

    void RenderFinished (IrGraphState state, IrGraphProcessor::BoxedBuffer render_result) override;

private:
    IOComponent io_component_;
    PanelComponent io_panel_ {io_component_};

    WaterfallComponent waterfall_component_;
    PanelComponent visualiser_panel_ {waterfall_component_};

    IrEngineComponent ir_engine_component_;
    PanelComponent ir_engine_panel_ {ir_engine_component_};
};
