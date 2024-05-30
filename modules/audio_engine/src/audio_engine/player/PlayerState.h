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
    std::optional<float> gain;
};

struct PlayerState
{
    Resources file = Resources::kSnare;
    bool is_looping = false;
    bool is_playing = false;
    float gain = 1.f;
};

};
