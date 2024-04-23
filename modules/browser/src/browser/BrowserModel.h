#pragma once

#include <immer/flex_vector.hpp>
#include <juce_core/juce_core.h>
#include <variant>

struct HomeView
{
};

struct ZoneView
{
};

struct Top10View
{
};

using BrowserView = std::variant<HomeView, ZoneView, Top10View>;

struct BrowserStackView
{
    juce::Uuid id;
    BrowserView view;
};

struct BrowserModel
{
    int stack_pointer = 0;
    immer::flex_vector<BrowserStackView> navigation_stack;
};