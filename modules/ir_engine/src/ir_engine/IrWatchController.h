#pragma once
#include "IrEngine.h"
#include "audio_engine/AudioEngine.h"
#include "ir_repository/project/ProjectIrLoadController.h"
#include "ir_repository/project/ProjectIrRepositoryModel.h"

#include <lager/reader.hpp>

class IrWatchController
{
public:
    IrWatchController (AudioEngine & audio_engine,
                       IrEngine & ir_engine,
                       ProjectIrLoadController & load_controller,
                       lager::reader<CurrentProjectIrOptional> current_ir_reader);

private:
    AudioEngine & audio_engine_;
    IrEngine & ir_engine_;
    ProjectIrLoadController & load_controller_;
    lager::reader<CurrentProjectIrOptional> current_ir_reader_;
};
