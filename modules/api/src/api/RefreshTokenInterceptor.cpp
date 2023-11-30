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
    if (account_reader_->account_status != AccountModel::AccountStatus::kAuthenticated)
        throw 0;

    // if (account_reader_->session.has_value ())
    // {
    //     auto account_session = account_reader_->session.value ();
    //     auto res = OidcApi::RefreshTokenRequest (account_session.tokens.refresh_token);
    // }

    // lager::watch (account_model_,
    //               [] (const AccountModel & account_model) {
    //
    //               });

    cpr::Response response = proceed (session);
    return response;
}