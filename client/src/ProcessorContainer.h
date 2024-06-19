#pragma once

#include "WithJuceEventLoop.h"
#include "audio_engine/AudioEngine.h"
#include "audio_engine/AudioGraph.h"
#include "audio_engine/CommandQueue.h"
#include "audio_engine/NotificationQueue.h"
#include "audio_engine/VisitorQueue.h"
#include "controllers/LoadController.h"
#include "ir_engine/IrController.h"
#include "ir_engine/IrEngine.h"
#include "zones_convolver/zones_convolver.h"

#include <juce_audio_processors/juce_audio_processors.h>

class ProcessorContainer
{
public:
    explicit ProcessorContainer (juce::AudioProcessor & audio_processor);
    void Prepare (double sampleRate, int samplesPerBlock, juce::AudioProcessor::BusesLayout layout);

    AudioGraphMetering input_graph_metering_;
    AudioGraphMetering output_graph_metering_;

    juce::AudioProcessorValueTreeState parameter_tree_;

    CommandQueue::VisitorQueue command_queue_;
    NotificationQueue::VisitorQueue notification_queue_;

    PlayerController player_controller_ {command_queue_};

    AudioGraph graph_;
    AudioEngine audio_engine_;

    juce::ThreadPool thread_pool_;

    IrEngine ir_engine_ {thread_pool_};
    IrController ir_controller_ {ir_engine_, parameter_tree_};
    LoadController load_controller_ {thread_pool_, ir_controller_};

    zones::ConvolutionEngine convolution_engine_ {thread_pool_};

private:
    void RegisterIrEngineListeners ();
};
