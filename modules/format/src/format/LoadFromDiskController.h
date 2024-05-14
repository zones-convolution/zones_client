#pragma once

#include "model/Action.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <lager/context.hpp>

class LoadFromDiskController
{
public:
    LoadFromDiskController (lager::context<Action> & context);
    void Load ();

private:
    ChannelFormat GetChannelFormatFromFile (juce::File & path);

    lager::context<Action> context_;
    std::unique_ptr<juce::FileChooser> directory_picker_;
};
