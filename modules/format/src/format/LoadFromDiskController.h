#pragma once

#include "model/Action.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <lager/context.hpp>

class LoadFromDiskController
{
public:
    LoadFromDiskController (lager::context<Action> & context, const lager::reader<Model> & model);
    void Load ();

private:
    ChannelFormat GetChannelFormatFromFile (juce::File & path);

    ChannelFormat GetChannelFormat (int num_channels);
    TargetFormat GetValidTargetFormat (int num_channels);

    ValidTargetFormatsReader valid_target_formats_reader_;

    lager::context<Action> context_;
    std::unique_ptr<juce::FileChooser> directory_picker_;
};
