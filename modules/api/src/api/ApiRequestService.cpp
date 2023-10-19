#include "ApiRequestService.h"

ApiRequestService::ApiRequestService (juce::Thread::Priority priority)
    : thread_pool_ (kNumberOfThreads, 0, priority)
{
}

juce::ThreadPoolJob * ApiRequestService::BeginRequest (const ApiRequest & api_request,
                                                       const ApiRequestJob::Callbacks & callbacks)
{
    auto requestJob = new ApiRequestJob (api_request, callbacks);
    thread_pool_.addJob (requestJob, true);
    return requestJob;
}

bool ApiRequestService::CancelRequest (juce::ThreadPoolJob * request_job)
{
    return thread_pool_.removeJob (request_job, true, kJobTimeout);
}

bool ApiRequestService::CancelAllRequests ()
{
    return thread_pool_.removeAllJobs (true, kJobTimeout);
}
