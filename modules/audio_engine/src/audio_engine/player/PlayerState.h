#pragma once
#include "optional"

namespace Player
{
enum Resource
{
    kSnare,
    kVoice
};

struct PlayerState
{
    Resource resource = Resource::kSnare;
    bool looping = false;
    bool playing = false;
    float gain = 0.6f;
};
}
