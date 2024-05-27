#pragma once
#include "optional"

namespace Player
{
enum Resources
{
    kSnare,
    kNumbers
};

struct PlayerStateOptional
{
    std::optional<Resources> file;
    std::optional<bool> is_looping;
    std::optional<bool> is_playing;
};

struct PlayerState
{
    Resources file;
    bool is_looping;
    bool is_playing;
};

};
