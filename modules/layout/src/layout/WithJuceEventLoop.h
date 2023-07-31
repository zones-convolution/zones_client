#pragma once

#include <functional>
#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>
#include <stdexcept>
#include <utility>

struct WithJuceEventLoop
{
private:
    std::shared_ptr<juce::ThreadPool> thread_pool_;

public:
    WithJuceEventLoop ()
    {
        thread_pool_ = std::make_shared<juce::ThreadPool> ();

        if (! juce::MessageManager::getInstance ()->isThisTheMessageThread ())
            throw std::runtime_error ("WithJuceEventLoop must be created in the message thread!");
    }

    ~WithJuceEventLoop ()
    {
        static constexpr int kTimeoutMs = 1000;
        jassert (thread_pool_->removeAllJobs (true, kTimeoutMs));
    }

    template <typename Fn>
    void async (Fn && fn)
    {
        thread_pool_->addJob (std::forward<Fn> (fn));
    }

    template <typename Fn>
    void post (Fn && fn)
    {
        juce::MessageManager::callAsync (std::forward<Fn> (fn));
    }

    void finish ()
    {
    }

    void pause ()
    {
    }

    void resume ()
    {
    }
};