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
    IrWatchController (AudioEngine & audio_engine,
                       IrEngine & ir_engine,
                       ProjectIrLoadController & load_controller,
                       lager::reader<ProjectIrRepositoryModel> project_ir_reader,
                       lager::reader<BoxedParameterModel> parameter_model_reader);

private:
    void WatchParameterModel ();
    void WatchCurrentIr ();
    void PerformRender ();

    AudioEngine & audio_engine_;
    IrEngine & ir_engine_;
    ProjectIrLoadController & load_controller_;
    lager::reader<CurrentProjectIrOptional> current_ir_reader_;
    lager::reader<BoxedParameterModel> parameter_model_reader_;
    IrGraphState current_graph_state_ {.room_size = 1.f};
    IrGraphProcessor::BoxedBuffer last_render_result_;
};
