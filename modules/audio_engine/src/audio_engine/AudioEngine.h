#pragma once
#include "CommandQueue.h"
#include "ir_repository/project/ProjectIrLoadController.h"
#include "model/Model.h"

#include <lager/reader.hpp>

class AudioEngine
{
public:
    explicit AudioEngine (CommandQueue::VisitorQueue & command_queue,
                          lager::reader<BoxedParameterModel> parameter_model_reader);
    void LoadIr (const IrData & ir_data);

private:
    CommandQueue::VisitorQueue & command_queue_;
    lager::reader<BoxedParameterModel> parameter_model_reader_;
};
