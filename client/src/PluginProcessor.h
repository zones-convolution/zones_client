#pragma once

#include "ir_repository/io/IrReader.h"
#include "ir_repository/io/IrWriter.h"
#include "ir_repository/project/ProjectIrImportController.h"
#include "ir_repository/project/ProjectIrLoadController.h"
#include "ir_repository/project/ProjectIrRepositoryAction.h"
#include "ir_repository/project/ProjectIrRepositoryModel.h"

#include <juce_audio_processors/juce_audio_processors.h>
#include <lager/event_loop/manual.hpp>
#include <lager/store.hpp>

class AudioPluginAudioProcessor : public juce::AudioProcessor
{
public:
    AudioPluginAudioProcessor ();
    ~AudioPluginAudioProcessor () override;

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
    std::string string_dep;
    lager::store<ProjectIrRepositoryAction, ProjectIrRepositoryModel> project_ir_store_ =
        lager::make_store<ProjectIrRepositoryAction> (ProjectIrRepositoryModel {},
                                                      lager::with_manual_event_loop {},
                                                      lager::with_deps (std::ref (string_dep)),
                                                      lager::with_reducer (Update));

    IrReader ir_reader_;
    IrWriter ir_writer_;

    ProjectIrLoadController project_ir_load_controller_ {project_ir_store_,
                                                         project_ir_store_,
                                                         ir_reader_};

    ProjectIrImportController project_ir_import_controller_ {project_ir_store_,
                                                             project_ir_store_,
                                                             ir_reader_,
                                                             ir_writer_};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessor)
};