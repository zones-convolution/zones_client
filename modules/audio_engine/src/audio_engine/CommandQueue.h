#pragma once
#include "VisitorQueue.h"
#include "player/PlayerState.h"

#include <variant>

struct CommandQueue
{
    struct UpdateParameters
    {
        float dry_wet_mix;
        float input_gain;
        float output_gain;
        float bass;
        float treble;
    };

    struct PlayCommand
    {
        Player::Resources file;
        bool looping;
        float gain;
    };

    struct StopCommand
    {
        int stop;
    };

    struct LoopCommand
    {
        bool loop;
    };

    struct FileCommand
    {
        Player::Resources file;
    };

    struct GainCommand
    {
        float gain;
    };

    using Commands = std::
        variant<UpdateParameters, PlayCommand, StopCommand, LoopCommand, FileCommand, GainCommand>;
    struct Visitor
    {
        virtual void operator() (const UpdateParameters & update_parameters) = 0;
        virtual void operator() (const PlayCommand & play_command) = 0;
        virtual void operator() (const StopCommand & stop_command) = 0;
        virtual void operator() (const LoopCommand & loop_command) = 0;
        virtual void operator() (const FileCommand & file_command) = 0;
        virtual void operator() (const GainCommand & gain_command) = 0;
    };

    using VisitorQueue = VisitorQueue<Commands, Visitor>;
};
