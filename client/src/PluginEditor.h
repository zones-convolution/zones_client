#pragma once

#include "PluginProcessor.h"
#include "ProjectImportComponent.h"
#include "zones_look_and_feel/LookAndFeel.h"

#include <juce_gui_basics/juce_gui_basics.h>

class AudioPluginAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor & processor,
                                              const lager::reader<ProjectIrRepositoryModel> & model,
                                              lager::context<ProjectIrRepositoryAction> & context);
    ~AudioPluginAudioProcessorEditor () override = default;

private:
    static constexpr float kPreferredAspectRatio = 3.f / 4.f;
    static constexpr int kWindowMinimumWidth = 600;
    static constexpr int kWindowMaxWidth = 2000;

    AudioPluginAudioProcessor & processor_;
    const lager::reader<ProjectIrRepositoryModel> model_;
    lager::context<ProjectIrRepositoryAction> context_;

    LookAndFeel look_and_feel_;
    ProjectImportComponent project_import_component_ {model_, context_};

    void resized () override;
    void paint (juce::Graphics & graphics) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};