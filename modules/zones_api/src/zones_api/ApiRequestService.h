#pragma once
#include "ApiRequest.h"
#include "ApiRequestJob.h"

#include <juce_core/juce_core.h>
namespace zones
{
class ApiRequestService
{
public:
    explicit ApiRequestService (juce::Thread::Priority priority = juce::Thread::Priority::normal);

    juce::ThreadPoolJob * beginRequest (const ApiRequest & apiRequest,
                                        const ApiRequestJob::Callbacks & callbacks);
    bool cancelRequest (juce::ThreadPoolJob * requestJob);
    bool cancelAllRequests ();

private:
    static constexpr int m_numberOfThreads = 10;
    static constexpr int m_jobTimeout = 100;

    juce::ThreadPool m_threadPool;
};

}
