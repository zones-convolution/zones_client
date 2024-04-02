#pragma once

#include "WithJuceEventLoop.h"
#include "audio_engine/AudioEngine.h"
#include "audio_engine/AudioGraph.h"
#include "audio_engine/CommandQueue.h"
#include "audio_engine/VisitorQueue.h"
#include "ir_engine/IrController.h"
#include "ir_engine/IrEngine.h"
#include "ir_repository/io/IrReader.h"
#include "model/Action.h"
#include "model/Model.h"
#include "zones_convolver/zones_convolver.h"

#include <juce_audio_processors/juce_audio_processors.h>
#include <lager/event_loop/manual.hpp>
#include <lager/store.hpp>

class ProcessorContainer
{
public:
    explicit ProcessorContainer (juce::AudioProcessor & audio_processor);

    AudioGraphMetering input_graph_metering_;
    AudioGraphMetering output_graph_metering_;

    AudioGraph graph_ {input_graph_metering_, output_graph_metering_, convolution_engine_};
    CommandQueue::VisitorQueue command_queue_ {graph_};

    juce::ThreadPool thread_pool_;
    IrEngine ir_engine_ {thread_pool_};

    juce::AudioProcessorValueTreeState parameter_tree_;
    AudioEngine audio_engine_;
    IrController ir_controller_ {ir_engine_, parameter_tree_};

    zones::ConvolutionEngine convolution_engine_ {thread_pool_};

    lager::store<Action, Model, Deps> store_ =
        lager::make_store<Action> (Model {},
                                   WithJuceEventLoop {thread_pool_},
                                   lager::with_reducer (Update),
                                   lager::with_deps (std::ref (ir_controller_)));

private:
    void RegisterIrEngineListeners ();
};
