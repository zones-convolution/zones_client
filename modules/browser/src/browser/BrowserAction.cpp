#include "BrowserAction.h"

static void PopStack (BrowserModel & model)
{
    model.navigation_stack = model.navigation_stack.take (model.stack_pointer + 1);
}

static void Jump (BrowserModel & model, int position)
{
    model.stack_pointer =
        std::clamp (position, 0, static_cast<int> (model.navigation_stack.size ()) - 1);
}

static void JumpToEnd (BrowserModel & model)
{
    Jump (model, static_cast<int> (model.navigation_stack.size ()) - 1);
}

static juce::Uuid LoadView (BrowserModel & model, BrowserView view)
{
    PopStack (model);
    auto view_id = juce::Uuid ();
    BrowserStackView stack_view {.id = view_id, .view = view};
    model.navigation_stack = model.navigation_stack.push_back (stack_view);
    JumpToEnd (model);
    return view_id;
}

BrowserResult UpdateBrowser (BrowserModel model, BrowserAction action)
{
    return std::visit (
        lager::visitor {
            [&] (const PopAction &) -> BrowserResult
            {
                PopStack (model);
                return {model, lager::noop};
            },
            [&] (const JumpAction & jump_action) -> BrowserResult
            {
                Jump (model, jump_action.position);
                return {model, lager::noop};
            },
            [&] (const LoadHomeAction & load_home_action) -> BrowserResult
            {
                auto view_id = LoadView (model, HomeView {});
                return {model, lager::noop};
            },
            [&] (const LoadZoneAction & load_zone_action) -> BrowserResult
            {
                auto view_id = LoadView (model, ZoneView {});
                return {model, lager::noop};
            },
            [&] (const LoadTop10Action & load_top_10_action) -> BrowserResult
            {
                auto view_id = LoadView (model, Top10View {});
                return {model, lager::noop};
            },
        },
        action);
}
