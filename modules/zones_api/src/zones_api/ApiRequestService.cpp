#include "ApiRequestService.h"

namespace zones
{
ApiRequestService::ApiRequestService (int threadPriority)
    : m_threadPool (m_numberOfThreads)
{
    m_threadPool.setThreadPriorities (threadPriority);
}

juce::ThreadPoolJob * ApiRequestService::beginRequest (const ApiRequest & apiRequest,
                                                       const ApiRequestJob::Callbacks & callbacks)
{
    auto requestJob = new ApiRequestJob (apiRequest, callbacks);
    m_threadPool.addJob (requestJob, true);
    return requestJob;
}

bool zones::ApiRequestService::cancelRequest (juce::ThreadPoolJob * requestJob)
{
    return m_threadPool.removeJob (requestJob, true, m_jobTimeout);
}

bool ApiRequestService::cancelAllRequests ()
{
    return m_threadPool.removeAllJobs (true, m_jobTimeout);
}

}