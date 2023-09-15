#pragma once

#include "audio_engine/AudioEngine.h"
#include "ir_engine/IrEngine.h"
#include "ir_repository/project/ProjectIrLoadController.h"
#include "ir_repository/project/ProjectIrRepositoryModel.h"
#include "model/ParameterTree.h"

#include <lager/reader.hpp>

class IrWatchController : public juce::AudioProcessorValueTreeState::Listener
{
public:
    IrWatchController (IrEngine & ir_engine,
                       ProjectIrLoadController & load_controller,
                       const lager::reader<ProjectIrRepositoryModel> & project_ir_reader,
                       juce::AudioProcessorValueTreeState & parameter_tree);
    ~IrWatchController () override = default;
    void parameterChanged (const juce::String & parameter_id, float new_value) override;

private:
    void UpdateParametersFromTree ();
    void WatchCurrentIr ();
    void PerformRender ();

    IrEngine & ir_engine_;
    ProjectIrLoadController & load_controller_;
    lager::reader<CurrentProjectIrOptional> current_ir_reader_;
    IrGraphState current_graph_state_ {};
    IrGraphProcessor::BoxedBuffer last_render_result_;
    juce::AudioProcessorValueTreeState & parameter_tree_;
};
