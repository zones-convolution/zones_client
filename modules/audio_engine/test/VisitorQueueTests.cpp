#include "audio_engine/VisitorQueue.h"

#include <catch2/catch_test_macros.hpp>
#include <variant>

struct Foo
{
    juce::Uuid identifier;
};

struct Bar
{
};

using Commands = std::variant<Foo, Bar>;

struct MockVisitor
{
    void operator() (const Foo & foo)
    {
        foo_visits.push_back (foo);
    }
    std::vector<Foo> foo_visits;

    void operator() (const Bar & bar)
    {
        bar_visits.push_back (bar);
    }
    std::vector<Bar> bar_visits;
};

using TestVisitorQueue = VisitorQueue<Commands, MockVisitor>;

SCENARIO ("queues commands", "[CommandQueue]")
{
    GIVEN ("a queued load ir command on a command queue")
    {
        MockVisitor visitor;
        TestVisitorQueue visitor_queue {visitor};

        auto foo = Foo {.identifier = juce::Uuid ()};
        visitor_queue.PushCommand (foo);

        WHEN ("servicing the queue")
        {
            visitor_queue.RTService ();
            THEN ("then the corresponding command was visited")
            {
                REQUIRE (visitor.foo_visits.size () == 1);
                auto last_visit = visitor.foo_visits.back ();
                REQUIRE (last_visit.identifier == foo.identifier);
            }
        }
    }

    GIVEN ("multiple queued commands on a visitor queue")
    {
        static constexpr int kNumFooCommandsToQueue = 20;
        MockVisitor visitor;
        TestVisitorQueue visitor_queue {visitor};

        std::vector<Foo> foos;
        for (auto command_index = 0; command_index < kNumFooCommandsToQueue; ++command_index)
            foos.push_back (Foo {.identifier = juce::Uuid ()});

        for (const auto & foo : foos)
            visitor_queue.PushCommand (foo);

        WHEN ("servicing the queue")
        {
            visitor_queue.RTService ();
            THEN ("commands are in the correct order")
            {
                for (auto command_index = 0; command_index < kNumFooCommandsToQueue;
                     ++command_index)
                {
                    auto & foo = visitor.foo_visits [command_index];
                    REQUIRE (foo.identifier == foos [command_index].identifier);
                }
            }
        }
    }
}
