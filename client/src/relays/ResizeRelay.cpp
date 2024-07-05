#include "ResizeRelay.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace juce
{
static void from_json (const json & data, Point<int> & point)
{
    data.at ("x").get_to (point.x);
    data.at ("y").get_to (point.y);
}
}

juce::WebBrowserComponent::Options
ResizeRelay::buildOptions (const juce::WebBrowserComponent::Options & initialOptions)
{
    return initialOptions
        .withNativeFunction ("begin_resize_native",
                             [&] (auto & var, auto complete)
                             {
                                 juce::Point<int> point;
                                 json::parse (var [0].toString ().toStdString ()).get_to (point);
                                 HandleDragBegin (point);
                                 JUCE_ASSERT_MESSAGE_THREAD;
                                 complete ({});
                             })
        .withNativeFunction ("end_resize_native",
                             [&] (auto & var, auto complete) { HandleDragEnd (); })
        .withNativeFunction ("resize_native",
                             [&] (auto & var, auto complete)
                             {
                                 juce::Point<int> point;
                                 json::parse (var [0].toString ().toStdString ()).get_to (point);
                                 HandleDrag (point);
                                 JUCE_ASSERT_MESSAGE_THREAD;
                                 complete ({});
                             });
}

void ResizeRelay::Setup (juce::Component * component,
                         juce::ComponentBoundsConstrainer * constrainer)
{
    component_ = component;
    constrainer_ = constrainer;
}

void ResizeRelay::HandleDragBegin (juce::Point<int> start_pos)
{
    if (component_ == nullptr)
    {
        jassertfalse;
        return;
    }

    original_bounds_ = component_->getBounds ();
    start_pos_ = start_pos;

    using Zone = juce::ResizableBorderComponent::Zone;
    const Zone zone {Zone::bottom | Zone::right};

    if (auto * peer = component_->getPeer ())
        if (&peer->getComponent () == component_)
            peer->startHostManagedResize (start_pos, zone);

    if (constrainer_ != nullptr)
        constrainer_->resizeStart ();
}

void ResizeRelay::HandleDragEnd ()
{
    if (constrainer_ != nullptr)
        constrainer_->resizeEnd ();
}

void ResizeRelay::HandleDrag (juce::Point<int> drag)
{
    if (component_ == nullptr)
    {
        jassertfalse;
        return;
    }

    auto delta = drag - start_pos_;
    auto r = original_bounds_.withSize (original_bounds_.getWidth () + delta.x,
                                        original_bounds_.getHeight () + delta.y);

    if (constrainer_ != nullptr)
        constrainer_->setBoundsForComponent (component_, r, false, false, true, true);
    else if (auto pos = component_->getPositioner ())
        pos->applyNewBounds (r);
    else
        component_->setBounds (r);
}
