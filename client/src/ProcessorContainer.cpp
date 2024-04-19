#include "ProcessorContainer.h"

#include "model/ParameterTree.h"

ProcessorContainer::ProcessorContainer (juce::AudioProcessor & audio_processor)
    : parameter_tree_ (audio_processor,
                       nullptr,
                       ParameterTree::kParameterTreeIdentifier,
                       ParameterTree::CreateParameterLayout ())
    , audio_engine_ (command_queue_, parameter_tree_, convolution_engine_, audio_processor)
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
