#pragma once
#include "IrGraphProcessor.h"

#include <immer/box.hpp>
#include <immer/set.hpp>

template <class T>
static inline void HashCombine (std::size_t & seed, const T & v)
{
    std::hash<T> hasher;
    seed ^= hasher (v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

class IrGraphCachePolicy
{
public:
    IrGraphCachePolicy () = default;
    [[nodiscard]] std::size_t GetHashForState (const IrGraphState & state) const;
    [[nodiscard]] bool StatesMatchWRTPolicy (const IrGraphState & s1,
                                             const IrGraphState & s2) const;
    bool operator== (const IrGraphCachePolicy & p) const;

    [[nodiscard]] IrGraphCachePolicy
    WithPolicyIdentifier (const std::string & policy_identifier) const;

    [[nodiscard]] IrGraphCachePolicy CacheParam1 (bool is_cached) const;
    [[nodiscard]] IrGraphCachePolicy CacheParam2 (bool is_cached) const;
    [[nodiscard]] IrGraphCachePolicy CacheParam3 (bool is_cached) const;

private:
    immer::box<std::string> policy_identifier_;
    bool param_1_cached_ = false;
    bool param_2_cached_ = false;
    bool param_3_cached_ = false;
};
