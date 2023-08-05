#pragma once

#include "audio_engine/AudioEngine.h"
#include "audio_engine/AudioGraph.h"
#include "audio_engine/CommandQueue.h"
#include "audio_engine/VisitorQueue.h"
#include "ir_repository/io/IrReader.h"
#include "ir_repository/io/IrWriter.h"
#include "ir_repository/project/ProjectIrImportController.h"
#include "ir_repository/project/ProjectIrLoadController.h"
#include "ir_repository/project/ProjectIrRepositoryAction.h"
#include "ir_repository/project/ProjectIrRepositoryModel.h"
#include "model/Action.h"
#include "model/Model.h"
#include "model/controllers/IrWatchController.h"

#include <lager/event_loop/manual.hpp>
#include <lager/store.hpp>

class ProcessorContainer
{
public:
    ProcessorContainer ();

    lager::store<Action, Model> store_ =
        lager::make_store<Action> (Model {},
                                   lager::with_manual_event_loop {},
                                   lager::with_reducer (Update));

    AudioGraphMetering audio_graph_metering_;
    AudioGraph graph_ {audio_graph_metering_};
    CommandQueue::VisitorQueue command_queue_ {graph_};

private:
    void RegisterIrEngineListeners ();

    IrReader ir_reader_;
    IrWriter ir_writer_;
    ProjectIrLoadController project_ir_load_controller_;
    ProjectIrImportController project_ir_import_controller_;
    AudioEngine audio_engine_;
    IrEngine ir_engine_;
    IrWatchController ir_watch_controller_;
};
