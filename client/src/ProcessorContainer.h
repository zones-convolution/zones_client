#pragma once

#include "WithJuceEventLoop.h"
#include "audio_engine/AudioEngine.h"
#include "audio_engine/AudioGraph.h"
#include "audio_engine/CommandQueue.h"
#include "audio_engine/NotificationQueue.h"
#include "audio_engine/VisitorQueueT.h"
#include "controllers/LoadController.h"
#include "controllers/WebLoadController.h"
#include "controllers/visualiser/VisualiserController.h"
#include "ir_engine/IrController.h"
#include "ir_engine/IrEngine.h"
#include "preferences/PreferencesController.h"
#include "zones_convolver/zones_convolver.h"

#include <juce_audio_processors/juce_audio_processors.h>

class ProcessorContainer
{
public:
    explicit ProcessorContainer (juce::AudioProcessor & audio_processor);
    void Prepare (double sampleRate, int samplesPerBlock, juce::AudioProcessor::BusesLayout layout);

    AudioGraphMetering input_graph_metering_;
    AudioGraphMetering output_graph_metering_;

    ParameterTree parameter_tree_;
    juce::AudioProcessorValueTreeState parameter_tree_state_;

    CommandQueue::VisitorQueue command_queue_;
    NotificationQueue::VisitorQueue notification_queue_;

    PlayerController player_controller_ {command_queue_};

    AudioGraph graph_;
    AudioEngine audio_engine_;

    juce::ThreadPool engine_pool_;
    IrEngine ir_engine_ {engine_pool_};

    IrController ir_controller_ {ir_engine_, parameter_tree_};

    juce::ThreadPool user_loading_pool_;
    LoadController load_controller_ {user_loading_pool_, ir_controller_};

    juce::ThreadPool web_loading_pool_;
    WebLoadController web_load_controller_ {web_loading_pool_, ir_controller_};

    juce::ThreadPool convolution_pool_;
    zones::ConvolutionEngine convolution_engine_ {convolution_pool_};

    VisualiserController visualiser_controller_ {};

    PreferencesController preferences_controller_ {ir_controller_};

private:
    void RegisterIrEngineListeners ();
    juce::AudioProcessorValueTreeState::ParameterLayout CreateParameterLayout ();
};
