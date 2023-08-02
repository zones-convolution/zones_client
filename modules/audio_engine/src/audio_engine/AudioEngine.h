#pragma once
#include "CommandQueue.h"
#include "ir_engine/IrEngine.h"
#include "ir_repository/project/ProjectIrLoadController.h"
#include "model/Model.h"

#include <lager/reader.hpp>

class AudioEngine : public IrEngine::Listener
{
public:
    explicit AudioEngine (CommandQueue::VisitorQueue & command_queue,
                          lager::reader<BoxedRealtimeParameterModel> realtime_parameter_reader);
    void LoadIr (const IrData & ir_data);
    void RenderFinished (IrGraphState state, IrGraphProcessor::BoxedBuffer render_result) override;

private:
    CommandQueue::VisitorQueue & command_queue_;
    lager::reader<BoxedRealtimeParameterModel> realtime_parameter_reader_;
};
