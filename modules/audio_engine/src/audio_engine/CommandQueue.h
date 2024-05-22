#pragma once
#include "VisitorQueue.h"

#include <variant>

struct CommandQueue
{
    struct UpdateParameters
    {
        float dry_wet_mix;
        float input_gain;
        float output_gain;
    };

    struct PlayCommand
    {
        int file;
        bool looping;
    };

    struct StopCommand
    {
        int stop;
    };

    using Commands = std::variant<UpdateParameters, PlayCommand, StopCommand>;
    struct Visitor
    {
        virtual void operator() (const UpdateParameters & update_parameters) = 0;
        virtual void operator() (const PlayCommand & play_command) = 0;
        virtual void operator() (const StopCommand & stop_command) = 0;
    };

    using VisitorQueue = VisitorQueue<Commands, Visitor>;
};
