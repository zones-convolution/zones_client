#pragma once
#include "ApiRequestService.h"
#include "OidcApi.h"
#include "account/AccountAction.h"
#include "account/AccountModel.h"

#include <lager/context.hpp>
#include <lager/reader.hpp>

class AuthenticatedApiRequestService : public ApiRequestService
{
public:
    AuthenticatedApiRequestService (
        lager::context<AccountAction> & account_context,
        lager::reader<AccountModel> & account_reader,
        juce::Thread::Priority priority = juce::Thread::Priority::normal);
    virtual ~AuthenticatedApiRequestService () = default;

    juce::ThreadPoolJob * BeginRequest (const ApiRequest & api_request,
                                        const ApiRequestJob::Callbacks & callbacks) override;

private:
    std::optional<OidcApi::DiscoverSuccessResponse> discover_success_response_;

    lager::context<AccountAction> account_context_;
    lager::reader<AccountModel> account_reader_;
};
