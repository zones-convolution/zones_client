#pragma once

#include "account/AccountAction.h"
#include "account/AccountModel.h"

#include <cpr/cpr.h>
#include <lager/context.hpp>
#include <lager/reader.hpp>

class RefreshTokenInterceptor final : public cpr::Interceptor
{
public:
    RefreshTokenInterceptor (lager::reader<AccountModel> & account_reader,
                             lager::context<AccountAction> & account_context);
    ~RefreshTokenInterceptor () override = default;
    cpr::Response intercept (cpr::Session & session) override;

private:
    lager::reader<AccountModel> account_reader_;
    lager::context<AccountAction> account_context_;
};