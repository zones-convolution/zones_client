#pragma once
#include <cpr/cpr.h>

namespace ApiUtils
{
template <class... Ts>
constexpr void ForwardSessionOptions (cpr::Session & session, Ts &&... ts)
{
    if constexpr (sizeof...(Ts) > 0)
        cpr::priv::set_option (session, std::forward<Ts> (ts)...);
}
};
