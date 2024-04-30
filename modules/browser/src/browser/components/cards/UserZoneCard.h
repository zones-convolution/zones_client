#pragma once

#include "browser/BrowserAction.h"
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
    UserZoneCard (const ZoneMetadata & zone_metadata,
                  const lager::reader<Model> & model,
                  lager::context<Action> & context,
                  lager::context<BrowserAction> & browser_context);
    void resized () override;

private:
    ZoneMetadata zone_metadata_;
    lager::context<Action> context_;
    CurrentIrReader ir_reader_;
    lager::context<BrowserAction> browser_context_;

    juce::Label zone_title_;
    IconButton load_ {"Load Zone", BoxIcons::kBxPlay};
    IconButton view_ {"View Zone", BoxIcons::kBxSushi};

    PanelComponent panel_ {
        PanelComponent::ColourPair {juce::Colours::darkmagenta, juce::Colours::darkorange},
        false};
};