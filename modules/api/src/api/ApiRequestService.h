#pragma once
#include "ApiRequest.h"
#include "ApiRequestJob.h"

#include <juce_core/juce_core.h>

class ApiRequestService
{
public:
    explicit ApiRequestService (juce::Thread::Priority priority = juce::Thread::Priority::normal);

    virtual juce::ThreadPoolJob * BeginRequest (const ApiRequest & api_request,
                                                const ApiRequestJob::Callbacks & callbacks);
    bool CancelRequest (juce::ThreadPoolJob * request_job);
    bool CancelAllRequests ();

protected:
    static constexpr int kNumberOfThreads = 10;
    static constexpr int kJobTimeout = 100;

    juce::ThreadPool thread_pool_;
};
