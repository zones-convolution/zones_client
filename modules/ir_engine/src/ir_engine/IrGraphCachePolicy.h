#pragma once
#include "IrGraphProcessor.h"

#include <immer/box.hpp>
#include <immer/flex_vector.hpp>

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

private:
    immer::box<std::string> policy_identifier_;
    immer::flex_vector<int IrGraphState::*> members_;
};
