#pragma once
#include "VisitorQueue.h"

#include <variant>

struct CommandQueue
{
    struct LoadIr
    {
        juce::AudioBuffer<float> * ir_buffer;
        double sample_rate;
    };

    struct UpdateParameters
    {
        float dry_wet_mix;
    };

    using Commands = std::variant<LoadIr, UpdateParameters>;
    struct Visitor
    {
        virtual void operator() (const LoadIr & load_ir) = 0;
        virtual void operator() (const UpdateParameters & update_parameters) = 0;
    };

    using VisitorQueue = VisitorQueue<Commands, Visitor>;
};
