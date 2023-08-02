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

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include <lager/event_loop/manual.hpp>
#include <lager/store.hpp>

class AudioPluginAudioProcessor : public juce::AudioProcessor
{
public:
    AudioPluginAudioProcessor ();
    ~AudioPluginAudioProcessor () override = default;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources () override;

    bool isBusesLayoutSupported (const BusesLayout & layouts) const override;

    void processBlock (juce::AudioBuffer<float> &, juce::MidiBuffer &) override;
    using AudioProcessor::processBlock;

    juce::AudioProcessorEditor * createEditor () override;
    bool hasEditor () const override;

    const juce::String getName () const override;

    bool acceptsMidi () const override;
    bool producesMidi () const override;
    bool isMidiEffect () const override;
    double getTailLengthSeconds () const override;

    int getNumPrograms () override;
    int getCurrentProgram () override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String & newName) override;

    void getStateInformation (juce::MemoryBlock & destData) override;
    void setStateInformation (const void * data, int sizeInBytes) override;

private:
    lager::store<Action, Model> store_ =
        lager::make_store<Action> (Model {},
                                   lager::with_manual_event_loop {},
                                   lager::with_reducer (Update));

    lager::context<ProjectIrRepositoryAction> project_ir_repository_context_ {store_};

    IrReader ir_reader_;
    IrWriter ir_writer_;
    ProjectIrLoadController project_ir_load_controller_;
    ProjectIrImportController project_ir_import_controller_;
    AudioGraph graph_;
    CommandQueue::VisitorQueue command_queue_ {graph_};
    AudioEngine audio_engine_;
    IrEngine ir_engine_;
    IrWatchController ir_watch_controller_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessor)
};