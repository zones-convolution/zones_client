#pragma once
#include "CommandQueue.h"
#include "ir_repository/project/ProjectIrLoadController.h"

#include <lager/reader.hpp>

class AudioEngine
{
public:
    explicit AudioEngine (CommandQueue::VisitorQueue & command_queue,
                          const ProjectIrLoadController & load_controller);

    void LoadIr (const IrData & ir_data);

private:
    CommandQueue::VisitorQueue & command_queue_;
    const ProjectIrLoadController & load_controller_;
    std::unique_ptr<IrData> ir_data_;
};
