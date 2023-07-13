#pragma once
#include "CommandQueue.h"
#include "ir_repository/project/ProjectIrLoadController.h"

#include <lager/reader.hpp>

class AudioEngine
{
public:
    explicit AudioEngine (CommandQueue & command_queue,
                          const ProjectIrLoadController & load_controller,
                          lager::reader<CurrentProjectIrOptional> current_ir_reader);

private:
    CommandQueue & command_queue_;
    lager::reader<CurrentProjectIrOptional> current_ir_reader_;
    const ProjectIrLoadController & load_controller_;
    std::unique_ptr<IrData> ir_data_;
};
