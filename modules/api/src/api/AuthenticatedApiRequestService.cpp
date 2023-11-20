#include "AuthenticatedApiRequestService.h"

AuthenticatedApiRequestService::AuthenticatedApiRequestService (
    lager::context<AccountAction> & account_context,
    lager::reader<AccountModel> & account_reader,
    juce::Thread::Priority priority)
    : ApiRequestService (priority)
    , account_context_ (account_context)
    , account_reader_ (account_reader)
{
}

juce::ThreadPoolJob *
AuthenticatedApiRequestService::BeginRequest (const ApiRequest & api_request,
                                              const ApiRequestJob::Callbacks & callbacks)
{
    if (! discover_success_response_)
    {
        OidcApi::D
    }

    auto request_job = new ApiRequestJob (api_request, callbacks);
    thread_pool_.addJob (request_job, true);
    return request_job;
}
