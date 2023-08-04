#pragma once

#include "ir_repository/project/ProjectIrRepositoryModel.h"
#include "zones_look_and_feel/LookAndFeel.h"
#include "zones_look_and_feel/components/PanelComponent.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <lager/reader.hpp>

class SidebarFooter : public juce::Component
{
public:
    SidebarFooter (const lager::reader<CurrentProjectIrOptional> & project_ir_reader);

    void resized () override;

private:
    lager::reader<CurrentProjectIrOptional> project_ir_reader_;

    static const PanelComponent::ColourPair kIrPanelGradient;
    juce::Label ir_label_;
    PanelComponent ir_label_panel_;

    void UpdateIrLabel ();
};
