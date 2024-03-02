#include "ProcessorContainer.h"

#include "model/ParameterTree.h"

ProcessorContainer::ProcessorContainer (juce::AudioProcessor & audio_processor)
    : parameter_tree_ (audio_processor,
                       nullptr,
                       ParameterTree::kParameterTreeIdentifier,
                       ParameterTree::CreateParameterLayout ())
    , project_ir_load_controller_ {store_ [&Model::project_ir_repository_model], store_, ir_reader_}
    , project_ir_import_controller_ {store_ [&Model::project_ir_repository_model],
                                     store_,
                                     ir_reader_,
                                     ir_writer_}
    , audio_engine_ (command_queue_, parameter_tree_, convolution_engine_)
    , ir_watch_controller_ (ir_engine_,
                            project_ir_load_controller_,
                            store_ [&Model::project_ir_repository_model],
                            parameter_tree_)
{
    RegisterIrEngineListeners ();

    const auto discovery_url = "https://cognito-idp.eu-west-2.amazonaws.com/eu-west-2_rzvTaRw3J";
    store_.dispatch (DiscoverOidcAction {.base_url = discovery_url});
}

void ProcessorContainer::RegisterIrEngineListeners ()
{
    auto & ir_engine_listeners = ir_engine_.GetListeners ();
    ir_engine_listeners.add (&audio_engine_);
}
