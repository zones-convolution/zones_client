#include "ProcessorContainer.h"

ProcessorContainer::ProcessorContainer ()
    : project_ir_load_controller_ {Model::ProjectIrRepositoryReader (store_), store_, ir_reader_}
    , project_ir_import_controller_ {Model::ProjectIrRepositoryReader (store_),
                                     store_,
                                     ir_reader_,
                                     ir_writer_}
    , audio_engine_ (command_queue_, Model::RealtimeParameterReader (store_))
    , ir_watch_controller_ (ir_engine_,
                            project_ir_load_controller_,
                            Model::ProjectIrRepositoryReader (store_),
                            Model::IrEngineParameterReader (store_))
{
    RegisterIrEngineListeners ();
}

void ProcessorContainer::RegisterIrEngineListeners ()
{
    auto & ir_engine_listeners = ir_engine_.GetListeners ();
    ir_engine_listeners.add (&audio_engine_);
}
