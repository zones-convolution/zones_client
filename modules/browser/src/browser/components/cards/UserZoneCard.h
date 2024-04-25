#pragma once

#include "look_and_feel/BoxIcons.h"
#include "look_and_feel/components/IconButton.h"
#include "look_and_feel/components/PanelComponent.h"
#include "model/Action.h"
#include "model/Model.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <lager/context.hpp>
#include <lager/reader.hpp>

class UserZoneCard : public juce::Component
{
public:
    UserZoneCard (const IrMetadata & ir_metadata,
                  const lager::reader<std::optional<IrMetadata>> & ir_reader);
    void resized () override;

    std::function<void ()> OnLoad;
    std::function<void ()> OnView;

private:
    IrMetadata ir_metadata_;
    lager::reader<std::optional<IrMetadata>> ir_reader_;

    juce::Label ir_title_;
    IconButton load_ {"Load Zone", BoxIcons::kBxPlay};
    IconButton view_ {"View Zone", BoxIcons::kBxSushi};

    PanelComponent panel_ {
        PanelComponent::ColourPair {juce::Colours::darkmagenta, juce::Colours::darkorange},
        false};
};