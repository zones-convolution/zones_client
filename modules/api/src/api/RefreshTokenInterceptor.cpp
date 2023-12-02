#include "RefreshTokenInterceptor.h"

#include "OidcApi.h"

RefreshTokenInterceptor::RefreshTokenInterceptor (lager::reader<AccountModel> & account_reader,
                                                  lager::context<AccountAction> & account_context)
    : account_reader_ (account_reader)
    , account_context_ (account_context)
{
}

cpr::Response RefreshTokenInterceptor::intercept (cpr::Session & session)
{
    account_context_.dispatch (RefreshTokenAction {});
    std::promise<std::string> bearer_promise;
    auto bearer_future = bearer_promise.get_future ();
    lager::watch (account_reader_,
                  [&] (const AccountModel & account_model)
                  { bearer_promise.set_value ("INVALID TOKEN"); });
    auto bearer = bearer_future.get ();
    session.SetBearer (cpr::Bearer {bearer});
    cpr::Response response = proceed (session);

    return response;
}