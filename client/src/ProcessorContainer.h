#pragma once

#include "WithJuceEventLoop.h"
#include "audio_engine/AudioEngine.h"
#include "audio_engine/AudioGraph.h"
#include "audio_engine/CommandQueue.h"
#include "audio_engine/VisitorQueue.h"
#include "ir_repository/io/IrReader.h"
#include "ir_repository/io/IrWriter.h"
#include "ir_repository/project/ProjectIrImportController.h"
#include "ir_repository/project/ProjectIrLoadController.h"
#include "model/Action.h"
#include "model/Model.h"
#include "model/controllers/IrWatchController.h"
#include "zones_convolver/zones_convolver.h"

#include <juce_audio_processors/juce_audio_processors.h>
#include <lager/event_loop/manual.hpp>
#include <lager/store.hpp>

class ProcessorContainer
{
public:
    explicit ProcessorContainer (juce::AudioProcessor & audio_processor);

    std::string dep_string_;
    juce::ThreadPool thread_pool_;

    lager::store<Action, Model, Deps> store_ =
        lager::make_store<Action> (Model {},
                                   WithJuceEventLoop {thread_pool_},
                                   lager::with_reducer (Update),
                                   lager::with_deps (std::ref (dep_string_)));

    juce::AudioProcessorValueTreeState parameter_tree_;
    AudioGraphMetering input_graph_metering_;
    AudioGraphMetering output_graph_metering_;
    ConvolutionEngine convolution_engine_ {thread_pool_};
    AudioGraph graph_ {input_graph_metering_, output_graph_metering_, convolution_engine_};
    CommandQueue::VisitorQueue command_queue_ {graph_};
    IrEngine ir_engine_;

private:
    void RegisterIrEngineListeners ();

    IrReader ir_reader_;
    IrWriter ir_writer_;
    ProjectIrLoadController project_ir_load_controller_;
    ProjectIrImportController project_ir_import_controller_;
    AudioEngine audio_engine_;
    IrWatchController ir_watch_controller_;
};
