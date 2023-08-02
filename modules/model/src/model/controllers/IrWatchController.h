#pragma once

#include "../ParameterModel.h"
#include "audio_engine/AudioEngine.h"
#include "ir_engine/IrEngine.h"
#include "ir_repository/project/ProjectIrLoadController.h"
#include "ir_repository/project/ProjectIrRepositoryModel.h"

#include <lager/reader.hpp>

class IrWatchController
{
public:
    IrWatchController (IrEngine & ir_engine,
                       ProjectIrLoadController & load_controller,
                       const lager::reader<ProjectIrRepositoryModel> & project_ir_reader,
                       const lager::reader<BoxedIrEngineParameterModel> & parameter_model_reader);

private:
    void WatchParameterModel ();
    void WatchCurrentIr ();
    void PerformRender ();

    IrEngine & ir_engine_;
    ProjectIrLoadController & load_controller_;
    lager::reader<CurrentProjectIrOptional> current_ir_reader_;
    lager::reader<BoxedIrEngineParameterModel> parameter_model_reader_;
    IrGraphState current_graph_state_ {};
    IrGraphProcessor::BoxedBuffer last_render_result_;
};
