#pragma once

#include <catch2/catch_test_macros.hpp>
#include <lager/context.hpp>
#include <lager/event_loop/manual.hpp>

template <typename ActionType, typename TargetActionType>
void RequireDispatchedAt (size_t position, const std::vector<ActionType> & actions)
{
    const auto * action = std::get_if<TargetActionType> (&actions [position]);
    REQUIRE (action != nullptr);
}

template <typename ActionType>
class TrackingContext
{
public:
    lager::with_manual_event_loop loop {};
    std::vector<ActionType> dispatched_actions_;
    lager::context<ActionType> context_ = {[&] (const auto & action)
                                           {
                                               dispatched_actions_.push_back (action);
                                               return lager::future {};
                                           },
                                           loop,
                                           {}};
};