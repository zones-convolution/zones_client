#pragma once
#include "optional"

namespace Player
{
enum Resource
{
    kSnare,
    kNumbers
};

struct PlayerState
{
    Resource resource = Resource::kSnare;
    bool looping = false;
    bool playing = false;
    float gain = 1.f;
};
}
