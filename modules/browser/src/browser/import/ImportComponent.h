#pragma once

#include "ImportIrComponent.h"
#include "ImportZoneComponent.h"
#include "look_and_feel/BoxIcons.h"
#include "look_and_feel/components/DividerComponent.h"
#include "look_and_feel/components/IconTextButton.h"

#include <juce_gui_basics/juce_gui_basics.h>

class ImportComponent : public juce::Component
{
public:
    ImportComponent ();
    void resized () override;

    std::function<void ()> OnSubmit;

private:
    juce::Label import_title_ {"Import Zone", "Import Zone"};
    DividerComponent top_divider_;

    ImportZoneComponent import_zone_;

    IconTextButton import_ir_button_ {"Import Ir", BoxIcons::kBxImport};
};
