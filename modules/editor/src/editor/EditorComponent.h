#pragma once

#include "ir_engine/IrEngine.h"
#include "model/ParameterAction.h"
#include "panels/IOPanel.h"
#include "panels/IrEnginePanel.h"
#include "panels/VisualiserPanel.h"
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
    IOPanel io_;
    PanelComponent io_panel_ {io_};

    VisualiserPanel visualiser_;
    PanelComponent visualiser_panel_ {visualiser_};

    IrEnginePanel ir_engine_;
    PanelComponent ir_engine_panel_ {ir_engine_};
};
