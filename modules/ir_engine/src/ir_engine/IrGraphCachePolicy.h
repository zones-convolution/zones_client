#pragma once
#include "IrGraphProcessor.h"

#include <functional>
#include <immer/box.hpp>
#include <immer/flex_vector.hpp>

template <class T>
static inline void HashCombine (std::size_t & seed, const T & v)
{
    std::hash<T> hasher;
    seed ^= hasher (v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template <typename StateType>
class IrGraphCachePolicy
{
public:
    IrGraphCachePolicy () = default;
    [[nodiscard]] std::size_t GetHashForState (const StateType & state) const
    {
        auto h = std::hash<std::string> {}(policy_identifier_);
        for (const auto int_handle : int_handles_)
            HashCombine (h, std::hash<int> {}(int_handle (state)));
        for (const auto float_handle : float_handles_)
            HashCombine (h, std::hash<float> {}(float_handle (state)));
        return h;
    }

    [[nodiscard]] bool StatesMatchWRTPolicy (const StateType & s1, const StateType & s2) const
    {
        for (const auto int_handle : int_handles_)
            if (! int_handle (s1) == int_handle (s2))
                return false;
        for (const auto float_handle : float_handles_)
            if (! juce::approximatelyEqual (float_handle (s1), float_handle (s2)))
                return false;
        return true;
    }

    bool operator== (const IrGraphCachePolicy & p) const
    {
        return policy_identifier_ == p.policy_identifier_ && int_handles_ == p.int_handles_ &&
               float_handles_ == p.float_handles_;
    }

    [[nodiscard]] IrGraphCachePolicy
    WithPolicyIdentifier (const std::string & policy_identifier) const
    {
        auto new_cache_policy = *this;
        new_cache_policy.policy_identifier_ = policy_identifier;
        return new_cache_policy;
    }

    template <typename ParamType>
    using StateHandle = std::function<ParamType (const StateType &)>;
    
    using IntStateHandle = StateHandle<int>;
    using FloatStateHandle = StateHandle<float>;

    [[nodiscard]] IrGraphCachePolicy WithCachedHandle (const IntStateHandle * param_handle) const
    {
        auto new_cache_policy = *this;
        new_cache_policy.int_handles_.push_back (param_handle);
        return new_cache_policy;
    }

    [[nodiscard]] IrGraphCachePolicy WithCachedHandle (const FloatStateHandle * param_handle) const
    {
        auto new_cache_policy = *this;
        new_cache_policy.float_handles_.push_back (param_handle);
        return new_cache_policy;
    }

private:
    immer::box<std::string> policy_identifier_;
    std::vector<const FloatStateHandle *> float_handles_;
    std::vector<const IntStateHandle *> int_handles_;
};
