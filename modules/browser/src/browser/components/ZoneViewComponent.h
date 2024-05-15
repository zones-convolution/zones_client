#pragma once

#include "../BrowserModel.h"
#include "format/IrFormatter.h"
#include "look_and_feel/BoxIcons.h"
#include "look_and_feel/components/IconTextButton.h"
#include "model/Action.h"

#include <juce_gui_basics/juce_gui_basics.h>

class ZoneViewComponent : public juce::Component
{
public:
    ZoneViewComponent (lager::context<Action> & context, const lager::reader<Model> & model);
    void Update (const ZoneView & zone_view);
    void resized () override;

private:
    void UpdateTargetFormats ();

    lager::context<Action> context_;

    ZoneMetadata zone_metadata_;
    IconTextButton load_ {"Load Zone", BoxIcons::kBxPlay};
    juce::ComboBox ir_selection_;
    juce::ComboBox target_format_selection_;
    juce::Label label_ {"Zone View", "Zone View"};

    ValidTargetFormatsReader valid_target_formats_reader_;
};
